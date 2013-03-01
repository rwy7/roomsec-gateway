/**
 * @file authorityadapter.h
 * Defines the base interface for interacting with an authority.
 */

#ifndef _ROOMSEC_AUTHORITYADAPTER_H_
#define _ROOMSEC_AUTHORITYADAPTER_H_

#include "gen-cpp/authorize_types.h"

namespace roomsec {

  /**
   * An authority object.  Authority objects implement decision making
   * regarding access to resources, on the basis of a set of
   * credentials.  Authority objects have no way to enforce policy.
   *
   * This class makes use of the Curiously recurring template pattern,
   * to achieve compile time polymorphism with adapter
   * implementations.
   * 
   */
  class AuthorityAdapter {
  public:
    AuthorityAdapter(void);
    virtual ~AuthorityAdapter();

    /**
     * Check the requirements of accessing a resource.
     * @param[out] requirements  List of required credentials to access resource.
     * @param[in]  resource      the string resource.
     */
    void checkRequirements(std::vector<iface::CredentialSpec>& requirements, const std::string& resource);

    /**
     * Attempt to obtain authorization from the authority.
     * @param request  The request for authorization
     */
    iface::AuthorizationReply::type authorize(const iface::AuthorizationRequest& request);

  private:

    /**
     * Implements the checkRequirements interface function.
     */
    virtual void checkRequirementsImpl(std::vector<iface::CredentialSpec>& requirements,
				       const std::string& resource);

    /**
     * Implements the authorize interface function.
     */
    virtual iface::AuthorizationReply::type authorizeImpl (const iface::AuthorizationRequest& request);
  };
}

#endif /* _ROOMSEC_AUTHORITYADAPTER_H_ */
