#include <iostream>
#include <string>
#include <exception>

#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>

#include "authorityadapter.h"
#include "thriftauthorityadapter.h"
#include "fingerprintauthnadapter.h"
#include "thriftfingerprintauthnadapter.h"

#include "replgateway.h"

#define AUTHZ_ADDR "192.168.0.194"
#define AUTHZ_PORT 9090

#define AUTHN_ADDR "172.17.144.152"
#define AUTHN_PORT 8080

namespace po = ::boost::program_options;

int init_logging();

int main (int argc, char *argv[]) {
  /* Initialize logging subsystem.
   * This is required by every class in gateway.so.
   */
  init_logging();
  log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("roomsec"));
  LOG4CXX_INFO(logger, "Initialized Logging");

  /* Authority Authorization information */
  int authzPort = AUTHZ_PORT;
  std::string authzAddr = AUTHZ_ADDR;

  /*  Fingerprint Authentication information */
  int authnPort = AUTHN_PORT;
  std::string authnAddr = AUTHN_ADDR;

  /*  Handle Program Options */
  po::options_description desc("Allowed Options");

  desc.add_options()
    ("help", "produce help message")
    ("fpauthn",  po::value<std::string>(), "Set the fingerprint authority server address");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

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
    log4cxx::BasicConfigurator::configure();
  }
  catch (std::exception& e) {
    result = EXIT_FAILURE;
  }
  return result;
}
