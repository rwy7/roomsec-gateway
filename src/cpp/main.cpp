#include "config.h"

#include <fstream>

#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>

#include <wiringPi/wiringPi.h>
#include <wiringPi/wiringPiSPI.h>

#include "libfprint/fprint.h"

#include "authorityadapter.h"
#include "thriftauthorityadapter.h"
#include "fingerprintauthnadapter.h"
#include "thriftfingerprintauthnadapter.h"

#include "gateway.h"
#include "stdgateway.h"
#include "replgateway.h"

#include "ioexpander.h"
#include "display.h"
#include "lcddisplay.h"
#include "buzzer.h"
#include "doorstatesensor.h"
#include "fingerprintscanner.h"
#include "blocksensor.h"
#include "mcp3008blocksensor.h"
#include "lock.h"

#include "main.h"


namespace po = ::boost::program_options;


/**
 * Define program options and parse the option sources.  Program
 * options can be processed from a configuration file, or parsed from
 * the command line. Options are processed here, and their values are
 * stored in a returned boost::variable_map.
 */
int storeOptions(int argc, char* argv[], po::variables_map & vm);

/**
 * Initialize the logging subsystem. 
 */
int initLogging(po::variables_map& vm);

/**
 * Initialize hardware systems and libraries. These are
 * initializations which must occur before specific hardware classes
 * are instantiated.
 */
int initHardware(po::variables_map& vm);
int cleanupHardware(po::variables_map const& vm);

boost::shared_ptr<roomsec::Gateway>
buildStdGateway(po::variables_map& vm);

boost::shared_ptr<roomsec::Gateway>
buildReplGateway(po::variables_map& vm);


log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("roomsec.main"));


int main (int argc, char *argv[]) {
  int retVal = 0;

  po::variables_map vm;
  if (storeOptions(argc, argv, vm) == 0 &&
      initLogging(vm) == 0) {

#ifdef ENABLE_GATEWAY
    LOG4CXX_DEBUG(logger, "ENABLE_GATEWAY is defined");
#else
    LOG4CXX_DEBUG(logger, "ENABLE_GATEWAY is NOT defined");
#endif /* ENABLE_GATEWAY */

    if(initHardware(vm)) {

      LOG4CXX_DEBUG(logger, "Building Gateway");
      boost::shared_ptr<roomsec::Gateway> gateway = buildStdGateway(vm);
      (*gateway)();
      cleanupHardware(vm);
    }
  }

  return retVal;
}


int storeOptions(int argc, char* argv[], po::variables_map & vm) {
  int retVal = 0;

  po::options_description generalOptions("Configuration Options");
  generalOptions.add_options()
    ("logconf",
         po::value<std::string>(),
         "log4cxx configuration file");

  po::options_description networkOptions("Networking Options");
  networkOptions.add_options()
    ("fpauthn",
         po::value<std::string>()->default_value("localhost"), 
         "Fingerprint authentication server address")
    ("fpauthn-port",
         po::value<int>()->default_value(2222),
         "Fingerprint authentication server port")
    ("authority",
         po::value<std::string>()->default_value("localhost"),   
         "Authority server address")
    ("authority-port",
         po::value<int>()->default_value(2223),
         "Authority server port");

  po::options_description commandLineOptions("Command Line Options");

  commandLineOptions.add_options()
    ("conf", po::value<std::string>(), "configuration file")
    ("help", "produce help message");

  commandLineOptions
    .add(generalOptions)
    .add(networkOptions);

  po::options_description fileOptions("Configuration File Options");
  fileOptions
    .add(generalOptions)
    .add(networkOptions);

  po::store(po::parse_command_line(argc, argv, commandLineOptions), vm);

  if(vm.count("conf")) {
    std::ifstream confFile;
    confFile.open(vm["conf"].as<std::string>());
    po::store(po::parse_config_file<char>(confFile, fileOptions), vm);
  }

  po::notify(vm);

  if(vm.count("help")) {
    std::cout << commandLineOptions << "\n";
    retVal = -2;
  }

  return retVal;
}


int initLogging(po::variables_map& vm) {
  int retVal = 0;

  if(vm.count("logconf")) {
    log4cxx::PropertyConfigurator::configure (vm["logconf"].as<std::string>());
    LOG4CXX_DEBUG(logger, "Initialized Logging with property configurator");
  }
  else {
    log4cxx::BasicConfigurator::configure();
    LOG4CXX_DEBUG(logger, "Initialized Logging with basic configurator");
  }
  
  return retVal;
}


int initHardware(po::variables_map& vm) {
  int retVal = 0;

  LOG4CXX_DEBUG(logger, "Initializing Hardware");

  /* Initialize libfprint for fingerprint scanning */
  LOG4CXX_DEBUG(logger, "Initializing libfprint");
  retVal = fp_init();
  if (retVal != 0) {
    LOG4CXX_ERROR(logger, "Failed to initialize libfprint");
  }
  else {
    fp_set_debug(3);
  }

  /* Initialize GPIO, SPI, and I2C Subsystems */

#ifdef ENABLE_GATEWAY
  LOG4CXX_DEBUG(logger, "Initializing WiringPi GPIO");
  if (wiringPiSetupGpio() != 0) {
    LOG4CXX_ERROR(logger, "WiringPi initialization failed");
    retVal = -1;
  }
  else {
    LOG4CXX_DEBUG(logger, "Initializing WiringPi SPI");
    if (wiringPiSPISetup(0, 500000) != 0){
      LOG4CXX_ERROR(logger, "WiringPi SPI initialization failed");
      retVal = -1;
    }
  }
#endif /* ENABLE_GATEWAY */

  return retVal;
}


int cleanupHardware(po::variables_map const& vm) {
  int error = 0;
  fp_exit();
  return error;
}

boost::shared_ptr<roomsec::Gateway>
buildStdGateway(po::variables_map& vm) {

  LOG4CXX_DEBUG(logger, "Constructing Standard Gateway StdGateway");
  roomsec::StdGateway::Builder builder;


  /* Authentication and Authorization Systems */

  /* Set up the authority and authentication adapters. Main is
     configuring to use networked processes communicating over a
     thrift connection. */

  /* Authority Authorization information */
  std::string authzAddr = vm["authority"].as<std::string>();
  int authzPort         = vm["authority-port"].as<int>();

  /*  Fingerprint Authentication information */
  std::string authnAddr = vm["fpauthn"].as<std::string>();
  int authnPort         = vm["fpauthn-port"].as<int>();

  LOG4CXX_DEBUG(logger, "Initializing AuthorityAdapter");
  boost::shared_ptr<roomsec::ThriftAuthorityAdapter>
    authzAdapter(new roomsec::ThriftAuthorityAdapter(authzAddr, authzPort));

  LOG4CXX_DEBUG(logger, "Initializing FingerprintAuthnAdapter");
  boost::shared_ptr<roomsec::ThriftFingerprintAuthnAdapter>
    authnAdapter(new roomsec::ThriftFingerprintAuthnAdapter(authnAddr, authnPort));

  builder
    .setAuthorityAdapter(authzAdapter)
    .setFingerprintAuthnAdapter(authnAdapter);


  /* User interface Systems */

  /* Initialize the screen hardware system */
  LOG4CXX_DEBUG(logger, "Starting LCD");

  LOG4CXX_DEBUG(logger, "IOExpander");
  boost::shared_ptr<roomsec::IOExpander> expander (new roomsec::IOExpander());
  expander->initialize(0x20);

  LOG4CXX_DEBUG(logger, "LCDDisplay");
  boost::shared_ptr<roomsec::LCDDisplay> display(new roomsec::LCDDisplay(expander));
  display->initialize();
  display->setBacklightPins(expander->GPIOB, 0x01, 0x02, 0x04);

  /* Initialize the buzzer */
  LOG4CXX_DEBUG(logger, "Initializing Buzzer");
  boost::shared_ptr<roomsec::Buzzer>
    buzzer(new roomsec::Buzzer(17));

  builder
    .setDisplay(display)
    .setBuzzer(buzzer);


  /* Door State Scanner */

  LOG4CXX_DEBUG(logger, "Initializing Door State Sensor");
  boost::shared_ptr<roomsec::DoorStateSensor>
    doorStateSensor(new roomsec::DoorStateSensor(18));

  /* Tailgate analyzer */

  LOG4CXX_DEBUG(logger, "Initializing Block Sensors");
  std::vector<roomsec::BlockSensor*> blockSensors;
  blockSensors.push_back(new roomsec::MCP3008BlockSensor(0));
  blockSensors.push_back(new roomsec::MCP3008BlockSensor(1));

  builder.setBlockSensors(blockSensors);


  /* Fingerprint Scanner */

  LOG4CXX_DEBUG(logger, "Initializing Fingerprint Scanner");
  roomsec::FingerprintScannerFactory fpScannerFact;

  if (fpScannerFact.getDeviceCount() < 1) {
    LOG4CXX_ERROR(logger, "No fingerprint scanner devices detected");
  }
  
  // Get first device found, no selection yet
  // This may be implemented as a program option eventually.

  boost::shared_ptr<roomsec::FingerprintScanner> fingerprintScanner =
    fpScannerFact.getFingerprintScanner(1);

  boost::shared_ptr<roomsec::Lock> lock(new roomsec::Lock(expander, expander->GPIOA, 0x80));

  builder
    .setLock(lock)
    .setDoorStateSensor(doorStateSensor)
    .setFingerprintScanner(fingerprintScanner);

  /* Build */

  LOG4CXX_DEBUG(logger, "Building NOW");
  return builder.build();
}

boost::shared_ptr<roomsec::Gateway>
buildReplGateway(po::variables_map& vm) {
  LOG4CXX_TRACE(logger, "Building ReplGateway");

  /* Authority Authorization information */
  int authzPort = AUTHZ_PORT;
  std::string authzAddr = AUTHZ_ADDR;

  /*  Fingerprint Authentication information */
  int authnPort = AUTHN_PORT;
  std::string authnAddr = AUTHN_ADDR;
  
  if (vm.count("fpauthn")) {
    authnAddr = vm["fpauthn"].as<std::string>();
  }

  if (vm.count("fpauthn-port")) {
    authnPort = vm["fpauthn-port"].as<int>();
  }

  boost::shared_ptr<roomsec::ThriftAuthorityAdapter>
    authzAdapter(new roomsec::ThriftAuthorityAdapter(authzAddr, authzPort));

  boost::shared_ptr<roomsec::ThriftFingerprintAuthnAdapter>
    authnAdapter(new roomsec::ThriftFingerprintAuthnAdapter(authnAddr, authnPort));

  roomsec::ReplGateway::Builder builder;
  
  boost::shared_ptr<roomsec::ReplGateway> gateway =
    builder
    .setAuthorityAdapter(authzAdapter)
    .setFingerprintAuthnAdapter(authnAdapter)
    .build();

  return gateway;
}
 
