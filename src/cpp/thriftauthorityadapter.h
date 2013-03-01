/**
 * @file thriftauthorityadapter.h
 * Defines an adapter which defers decisions to a networked authority server.
 */

#ifndef _ROOMSEC_THRIFTAUTHORITYADAPTER_H_
#define _ROOMSEC_THRIFTAUTHORITYADAPTER_H_

#include "authorityadapter.h"

/*
class apache::thrift::transport::TSocket;
class apache::thrift::transport::TTransport;
class apache::thrift::protocol::TProtocol;
*/

namespace roomsec {
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

    void checkRequirementsImpl(std::vector<iface::CredentialSpec>& requirements,
			       const std::string& resource);

    iface::AuthorizationReply::type authorizeImpl(const iface::AuthorizationRequest& request);
  };
}

#endif /* _ROOMSEC_THRIFTAUTHORITYADAPTER_H_ */
