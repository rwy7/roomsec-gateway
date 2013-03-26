#include <iostream>
#include <string>
#include <exception>

#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

#include "authorityadapter.h"
#include "thriftauthorityadapter.h"
#include "fingerprintauthnadapter.h"
#include "thriftfingerprintauthnadapter.h"

#include "replgateway.h"
#include "ioexpander.h"
#include "display.h"
#include "lcddisplay.h"

#include "wiringPi/wiringPi.h"

#define AUTHZ_ADDR "192.168.0.194"
#define AUTHZ_PORT 9090

#define AUTHN_ADDR "172.17.144.152"
#define AUTHN_PORT 8080

namespace po = ::boost::program_options;

int init_logging();
log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("roomsec.main"));

int main (int argc, char *argv[]) {

  // Test the net logger
  log4cxx::LoggerPtr netLogger(log4cxx::Logger::getLogger("roomsec.net"));
  LOG4CXX_INFO(netLogger, "Hello, world!");

#ifdef ENABLE_GATEWAY
  if (wiringPiSetup () == -1) {
    printf("went done bad");
    return -1;
  }
#endif

  /* BEGIN TEMP {{{ */

  printf("starting lcd\n");
  boost::shared_ptr<roomsec::IOExpander> expander (new roomsec::IOExpander());
  expander->initialize(0x20);
  roomsec::LCDDisplay disp = roomsec::LCDDisplay(expander);
  disp.initialize();

  disp.putStr("AY > RY");
  printf("ending LCD\n");

  /*  }}} END TEMP */

  /* Authority Authorization information */
  int authzPort = AUTHZ_PORT;
  std::string authzAddr = AUTHZ_ADDR;

  /*  Fingerprint Authentication information */
  int authnPort = AUTHN_PORT;
  std::string authnAddr = AUTHN_ADDR;

  /*  Handle Program Options */
  po::options_description desc("Allowed Options");

  desc.add_options()
    ("logconf", po::value<std::string>(), "log4cxx configuration file")
    ("help", "produce help message")
    ("fpauthn",  po::value<std::string>(), "Set the fingerprint authority server address");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  
  if(vm.count("logconf")) {
    log4cxx::PropertyConfigurator::configure (vm["logconf"].as<std::string>());
    LOG4CXX_DEBUG(logger, "Initialized Logging with property configurator");
  }
  else {
    log4cxx::BasicConfigurator::configure();
    LOG4CXX_INFO(logger, "Initialized Logging with basic configurator");
  }

  if(vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }

  if (vm.count("fpauthn")) {

  }

  boost::shared_ptr<roomsec::ThriftAuthorityAdapter>
    authzAdapter(new roomsec::ThriftAuthorityAdapter(authzAddr, authzPort));

  boost::shared_ptr<roomsec::ThriftFingerprintAuthnAdapter>
    authnAdapter(new roomsec::ThriftFingerprintAuthnAdapter(authnAddr, authnPort));

  roomsec::ReplGateway::Builder builder;

  builder
    .authorityAdapter(authzAdapter)
    .fingerprintAuthnAdapter(authnAdapter)
    .build()
    ->start();

  return 0;
}


int init_logging() {
  int result = EXIT_SUCCESS;
  try {
    
  }
  catch (std::exception& e) {
    result = EXIT_FAILURE;
  }
  return result;
}
