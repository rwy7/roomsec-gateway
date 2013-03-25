#include <iostream>
#include <log4cxx/logger.h>
#include "gen-cpp/authorize_types.h"
#include "authorityadapter.h"

namespace roomsec {
  log4cxx::LoggerPtr AuthorityAdapter::logger(log4cxx::Logger::getLogger("roomsec.authority"));

  AuthorityAdapter::AuthorityAdapter(void) {
    LOG4CXX_TRACE(logger, "Enter");
  }

  AuthorityAdapter::~AuthorityAdapter(void) {
    LOG4CXX_TRACE(logger, "Enter");
  }

  void 
  AuthorityAdapter::checkRequirements(std::vector<iface::CredentialSpec>& requirements,
				      const std::string& resource) {
    LOG4CXX_TRACE(logger, "Enter");
    this->checkRequirementsImpl(requirements, resource);
    return;
  }

  iface::AuthorizationReply::type
  AuthorityAdapter::authorize(const iface::AuthorizationRequest& request) {
    LOG4CXX_TRACE(logger, "Enter");
    std::cout << "authorize called";
    return this->authorizeImpl(request);
  }
}
