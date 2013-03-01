#include <iostream>
#include "gen-cpp/authorize_types.h"
#include "authorityadapter.h"

namespace roomsec {
  AuthorityAdapter::AuthorityAdapter(void) {
    std::cout << "Authority adapter initialized";
  }

  AuthorityAdapter::~AuthorityAdapter(void) {
    std::cout << "Authority adapter destroyed";
  }

  void 
  AuthorityAdapter::checkRequirements(std::vector<iface::CredentialSpec>& requirements,
				      const std::string& resource) {
    std::cout << "checkRequirements called";
    this->checkRequirementsImpl(requirements, resource);
    return;
  }

  iface::AuthorizationReply::type
  AuthorityAdapter::authorize(const iface::AuthorizationRequest& request) {
    std::cout << "authorize called";
    return this->authorizeImpl(request);
  }
}
