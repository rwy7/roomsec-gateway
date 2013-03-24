#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>

#include "authorityadapter.h"
#include "thriftauthorityadapter.h"
#include "fingerprintauthnadapter.h"
#include "thriftfingerprintauthnadapter.h"

#include "replgateway.h"
#include "ioexpander.h"
#include "display.h"
#include "lcddisplay.h"

#define AUTHZ_ADDR "192.168.0.194"
#define AUTHZ_PORT 9090

#define AUTHN_ADDR "172.17.144.152"
#define AUTHN_PORT 8080

namespace po = boost::program_options;

int init_logging();

int main (int argc, char *argv[]) {
/* TEMP */
if (wiringPiSetup () == -1) {
printf("went done bad");
return -1;
}

boost::shared_ptr<roomsec::IOExpander> expander (new roomsec::IOExpander());
expander->initialize(0x20);
roomsec::LCDDisplay disp = roomsec::LCDDisplay(expander);
disp.initialize();


/* END TEMP */

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
  // TODO: Configure Logging
  return 0;
}
