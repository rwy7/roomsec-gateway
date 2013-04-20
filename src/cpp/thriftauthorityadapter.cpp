#include <iostream>

#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include <log4cxx/logger.h>
#include "gen-cpp/Authority.h"
#include "thriftauthorityadapter.h"

namespace roomsec {
  namespace thrift = ::apache::thrift;
  namespace transport = ::apache::thrift::transport;
  namespace protocol = ::apache::thrift::protocol;

  static log4cxx::LoggerPtr logger2 = 
    log4cxx::Logger::getLogger("roomsec.authorityadapter");

  ThriftAuthorityAdapter::ThriftAuthorityAdapter(const std::string& authorityServer, 
						 int authorityPort)
    : socket(new transport::TSocket(authorityServer, authorityPort)),
      transport(new transport::TBufferedTransport(socket)),
      protocol(new protocol::TBinaryProtocol(transport)),
      client(protocol)
  {
    LOG4CXX_DEBUG(logger2, "ThriftAuthorityAdapter initialized");
  }

  ThriftAuthorityAdapter::~ThriftAuthorityAdapter() {
    LOG4CXX_DEBUG(logger2, "ThriftAuthorityAdapter destroyed");
  }

  void
  ThriftAuthorityAdapter::checkRequirementsImpl(std::vector<iface::CredentialSpec>& requirements,
						const std::string& resource) {
    LOG4CXX_DEBUG(logger2, "check requirements");
    this->transport->open();
    LOG4CXX_DEBUG(logger2, "transport open");
    this->client.checkRequirements(requirements, resource);
    LOG4CXX_DEBUG(logger2, "got results");
    this->transport->close();
    LOG4CXX_DEBUG(logger2, "transport closed");
    return;
  }

  iface::AuthorizationReply::type
  ThriftAuthorityAdapter::authorizeImpl(const iface::AuthorizationRequest& request) {
    LOG4CXX_DEBUG(logger2, "authorizing");
    this->transport->open();
    LOG4CXX_DEBUG(logger2, "transport open");
    iface::AuthorizationReply::type reply = this->client.authorize(request);
    LOG4CXX_DEBUG(logger2, "got reply");
    this->transport->close();
    LOG4CXX_DEBUG(logger2, "transport closed");
    return reply;
  }
}
