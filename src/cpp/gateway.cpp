#include "gateway.h"

namespace roomsec {

  Gateway::~Gateway() {
  }

  void
  Gateway::setAuthorityAdapter(boost::shared_ptr<AuthorityAdapter> authzAdapter){
    this->authzAdapter = authzAdapter;
  }

  void
  Gateway::setFingerprintAuthnAdapter
  (boost::shared_ptr<FingerprintAuthnAdapter> authnAdapter) {
    this->authnAdapter = authnAdapter;
  }
}
