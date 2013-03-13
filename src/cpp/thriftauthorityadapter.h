/**
 * @file thriftauthorityadapter.h
 * Defines an adapter which defers decisions to a networked authority server.
 */

#ifndef _ROOMSEC_THRIFTAUTHORITYADAPTER_H_
#define _ROOMSEC_THRIFTAUTHORITYADAPTER_H_

/*
#include <boost/shared_ptr.hpp>
#include <transport/TSocket.h>
#include <transport/TTransport.h>
#include <protocol/TProtocol.h>
*/

#include "authorityadapter.h"

/* Thrift dependencies -- forward declarations */
namespace apache { namespace thrift {
    namespace transport {
      class TSocket;
      class TTransport;
    }
    namespace protocol {
      class apache::thrift::protocol::TProtocol;
    }
  }
}

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
