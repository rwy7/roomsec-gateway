#ifndef _ROOMSEC_FINGERPRINTAUTHNADAPTER_
#define _ROOMSEC_FINGERPRINTAUTHNADAPTER_

#include "gen-cpp/authorize_types.h"

namespace roomsec {
  class FingerprintAuthnAdapter {

  public:
    //FingerprintAuthnAdapter()  = 0;
    virtual ~FingerprintAuthnAdapter() {};

    /**
     * Authenticate a fingerprint object.  This method maps an input
     * fingerprint to a matching identity, andthen provides a
     * credential object certifiying that identity.
     * @param[out] credential   A temporary credential certifying an identity.
     * @param[in]  fingerprint  The fingerprint which identifies the user.
     */
    void authenticate(iface::Credential& credential, const std::string& fingerprint);

  private:
    /**
     * Implements the authenticate interface function.
     * Intended to be overridden by subclass implementations.
     */
    virtual void authenticateImpl(iface::Credential& credential, const std::string& fingerprint) = 0;
  };
}

#endif /* _ROOMSEC_FINGERPRINTAUTHNADAPTER_ */
