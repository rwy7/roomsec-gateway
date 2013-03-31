#include "config.h"

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/assert.hpp>

#include "authorityadapter.h"
#include "fingerprintauthnadapter.h"
#include "replgateway.h"

#include "fingerprintscanner.h"

/*
 * The following includes should be removed with the fingerprint
 * scanner wrapper.
 */

#include "gen-cpp/authorize_types.h"

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libfprint/fprint.h"
}

namespace roomsec {

  /*****************************************
   * ReplGateway code starts here.
   */

  ReplGateway::ReplGateway() {
  }


  ReplGateway::~ReplGateway() {
  }


  void
  ReplGateway::init() {

  }


  void
  ReplGateway::begin() {
    this->start_repl();
  }


  void
  ReplGateway::start_repl() {

    std::cout << "RoomSec Gateway\n";

    bool quit = false;
    iface::Credential credential;
    std::string input;

    std::cout << "('help' for commands)\n";

    while(!quit) {
      std::cout << "Input?\n";
      std::getline(std::cin, input);

      if (!input.compare("quit")) {
	quit = true;
      }

      if (!input.compare("authn")) {
	std::cout << "Authenticating\n";

	FingerprintScannerFactory fpScannerFact;

	if (fpScannerFact.getDeviceCount() < 1) {
	  fprintf(stderr, "No devices detected");
	}

	boost::shared_ptr< FingerprintScanner > fpScanner =
	  fpScannerFact.getFingerprintScanner(1);

	boost::shared_ptr< Fingerprint > fp = fpScanner->scanFingerprint();

	authnAdapter
	  ->authenticate(credential, 
			 fp->serialize());

	std::cout << "Recieved: " << credential.token 
		  << " User: " << credential.userid << "\n";
      }
    }

    if(!input.compare("authz")) {
      std::cout << "Not yet implemented\n";
    }

    if(!input.compare("check"))  {
      std::string resource;
      std::cout << "resource: ";
      std::cin >> resource;
	
      std::cout << "checking\n";
      std::vector<iface::CredentialSpec> requiredCreds;
      authzAdapter->checkRequirements(requiredCreds, resource);
	
      std::cout << "requires credentials:\n";
      for(std::vector<iface::CredentialSpec>::iterator it =  requiredCreds.begin(); 
	  it != requiredCreds.end(); ++it) {
	std::cout << "  " << "provider: " << it->provider
		  <<" mechanism: " << it->mechanism << "\n";
      }
    }

    if(!input.compare("help")) {
      std::cout << "check     - check credential requirements for resource\n"
		<< "authn     - authenticate with fingerprint\n"
		<< "authz     - authorize \n";
    }
  }


  /*
   * ReplGateway Builder
   */

  boost::shared_ptr<ReplGateway >
  ReplGateway::Builder::build() {
    BOOST_ASSERT(this->authorityAdapter != NULL);
    BOOST_ASSERT(this->fingerprintAuthnAdapter != NULL);

    boost::shared_ptr<ReplGateway> replGateway(new ReplGateway);
    replGateway->setAuthorityAdapter(this->authorityAdapter);
    replGateway->setFingerprintAuthnAdapter(this->fingerprintAuthnAdapter);
    
    return replGateway;
  }
}
