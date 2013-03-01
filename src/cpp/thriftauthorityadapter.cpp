#include <iostream>

#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "gen-cpp/Authority.h"
#include "thriftauthorityadapter.h"

namespace roomsec {
  namespace thrift = ::apache::thrift;
  namespace transport = ::apache::thrift::transport;
  namespace protocol = ::apache::thrift::protocol;

  ThriftAuthorityAdapter::ThriftAuthorityAdapter(const std::string& authorityServer, 
						 int authorityPort) : 
    socket(new transport::TSocket(authorityServer, authorityPort)),
    transport(new transport::TBufferedTransport(socket)),
    protocol(new protocol::TBinaryProtocol(transport)),
    client(protocol) {
    std::cout << "ThriftAuthorityAdapter initialized" << std::endl;
  }

  ThriftAuthorityAdapter::~ThriftAuthorityAdapter() {
    std::cout << "ThriftAuthorityAdapter destroyed" << std::endl;
  }

  void
  ThriftAuthorityAdapter::checkRequirementsImpl(std::vector<iface::CredentialSpec>& requirements,
						const std::string& resource) {
    this->transport->open();
    this->client.checkRequirements(requirements, resource);
    this->transport->close();
    return;
  }

  iface::AuthorizationReply::type
  ThriftAuthorityAdapter::authorizeImpl(const iface::AuthorizationRequest& request) {
    this->transport->open();
    iface::AuthorizationReply::type reply = this->client.authorize(request);
    this->transport->close();
    return reply;
  }
}
