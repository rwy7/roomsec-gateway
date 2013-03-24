/* -*- Mode: c++ -*- */

/**
 * @file thriftauthorityadapter.h
 * Defines an adapter which defers decisions to a networked authority server.
 */

#ifndef _ROOMSEC_THRIFTAUTHORITYADAPTER_H_
#define _ROOMSEC_THRIFTAUTHORITYADAPTER_H_

#include "gen-cpp/Authority.h"
#include "authorityadapter.h"

/* Thrift dependencies -- forward declarations */
namespace apache { namespace thrift {
    namespace transport {
      class TSocket;
      class TTransport;
    }
    namespace protocol {
      class TProtocol;
    }
  }
}

namespace roomsec {

  /**
   * An adapter to a remotely placed authority process.  Communication
   * between the adapter and the authority occurs over an ip-based
   * thrift protocol.
   *
   * The exact thrift protocol is unspecified, leaving the decision up
   * to the policy of higher level modules.
   */
  class ThriftAuthorityAdapter: public AuthorityAdapter {
  public:
    ThriftAuthorityAdapter(const std::string& authorityServer,
			   int authorityPort);

    virtual ~ThriftAuthorityAdapter();
    
  protected:
    boost::shared_ptr<apache::thrift::transport::TSocket> socket;
    boost::shared_ptr<apache::thrift::transport::TTransport> transport;
    boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol;
    iface::AuthorityClient client;

  private:

    /**
     * Obtain a record of required credentials for a resource.
     *
     * This resource is generated and produced by a remote authority agent.
     *
     * @param[out] requirements  A specification of required credentials.
     * @param[in]  resource      a URL which uniquely identifies the resource.
     */
    void checkRequirementsImpl(std::vector<iface::CredentialSpec>& requirements,
			       const std::string& resource);

    /**
     * Defer an authorization decision to a remotely placed authority process.
     */
    iface::AuthorizationReply::type authorizeImpl(const iface::AuthorizationRequest& request);
  };
}

#endif /* _ROOMSEC_THRIFTAUTHORITYADAPTER_H_ */
