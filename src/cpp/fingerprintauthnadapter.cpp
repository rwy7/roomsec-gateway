#include <iostream>
#include "gen-cpp/authorize_types.h"
#include "fingerprintauthnadapter.h"

namespace roomsec {

  void
  FingerprintAuthnAdapter::authenticate(iface::Credential& credential, 
					const std::string& fingerprint) {
    std::cout << "Authenticate called" << std::endl;
    this->authenticateImpl(credential, fingerprint);
    return;
  }
}
