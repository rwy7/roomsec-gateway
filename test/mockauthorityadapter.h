#ifndef _ROOMSEC_MOCKAUTHORITYADAPTER_H_
#define _ROOMSEC_MOCKAUTHORITYADAPTER_H_

#include "authorityadapter.h"

namespace roomsec {
  class MockAuthorityAdapter : public AuthorityAdapter {
  public:
    MOCK_METHOD2(checkRequirementsImpl, void(std::vector<iface::CredentialSpec>& requirements, const std::string& resource));
    MOCK_METHOD1(authorizeImpl, iface::AuthorizationReply::type(const iface::AuthorizationRequest& request));
  };
}

#endif /* _ROOMSEC_MOCKAUTHORITYADAPTER_H_ */
