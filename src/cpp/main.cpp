#include "config.h"
#include <iostream>
#include <string>
#include <exception>

#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

#include <wiringPi/wiringPi.h>

#include "authorityadapter.h"
#include "thriftauthorityadapter.h"
#include "fingerprintauthnadapter.h"
#include "thriftfingerprintauthnadapter.h"
#include "replgateway.h"
#include "ioexpander.h"
#include "display.h"
#include "lcddisplay.h"

#include "main.h"

namespace po = ::boost::program_options;

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("roomsec.main"));

int main (int argc, char *argv[]) {
  int retVal = 0;

  po::variables_map vm;
  if (storeOptions(argc, argv, vm) == 0 && initLogging(vm) == 0) {

#ifdef ENABLE_GATEWAY
    LOG4CXX_DEBUG(logger, "ENABLE_GATEWAY is defined");
#else
    LOG4CXX_DEBUG(logger, "ENABLE_GATEWAY is NOT defined");
#endif /* ENABLE_GATEWAY */

    initHardware(vm);

    // Test the net logger
    log4cxx::LoggerPtr netLogger(log4cxx::Logger::getLogger("roomsec.net"));
    LOG4CXX_INFO(netLogger, "Hello, world!");


    /* Set up the authority and authentication adapters. Main is
       configuring to use networked processes communicating over a
       thrift connection. */

    /* Initialize the screen hardware system
     */
    LOG4CXX_DEBUG(logger, "Starting LCD");

    boost::shared_ptr<roomsec::IOExpander> expander (new roomsec::IOExpander());
    expander->initialize(0x20);

    boost::shared_ptr<roomsec::LCDDisplay> disp(new roomsec::LCDDisplay(expander));
    disp->initialize();

    disp->putStr("    RoomSec");
    disp->setDisplay(1, 0);
    disp->putStr("initializing...");
    disp->setBacklightPins(expander->GPIOB, 0x01, 0x02, 0x04);
    disp->setBacklightColor(disp->blue);

    /*  }}} END TEMP */

    buildReplGateway(vm)->start();
  }

  return retVal;
}


int storeOptions(int argc, char* argv[], po::variables_map & vm) {
  int retVal = 0;
  po::options_description desc("Allowed Options");

  desc.add_options()
    ("logconf", po::value<std::string>(), "log4cxx configuration file")
    ("help", "produce help message")
    ("fpauthn",  po::value<std::string>(), "Set the fingerprint authority server address");

  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help")) {
    std::cout << desc << "\n";
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

#ifdef ENABLE_GATEWAY
  if (wiringPiSetup() == -1) {
    LOG4CXX_ERROR(logger, "Wiring PI initialization failed");
    retVal =  -1;
  }
#endif /* ENABLE_GATEWAY */

  return retVal;
}

boost::shared_ptr<roomsec::Gateway>
buildStdGateway(po::variables_map& vm) {
  LOG4CXX_ERROR(logger, "Not yet implemented");
  return boost::shared_ptr<roomsec::Gateway>();
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
    // Do something?
  }

  boost::shared_ptr<roomsec::ThriftAuthorityAdapter>
    authzAdapter(new roomsec::ThriftAuthorityAdapter(authzAddr, authzPort));

  boost::shared_ptr<roomsec::ThriftFingerprintAuthnAdapter>
    authnAdapter(new roomsec::ThriftFingerprintAuthnAdapter(authnAddr, authnPort));

  roomsec::ReplGateway::Builder builder;
  
  boost::shared_ptr<roomsec::ReplGateway> gateway = 
    builder
    .authorityAdapter(authzAdapter)
    .fingerprintAuthnAdapter(authnAdapter)
    .build();

  return gateway;
}
