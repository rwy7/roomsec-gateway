#include <iostream>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include "thriftfingerprintauthnadapter.h"

namespace roomsec {
  namespace thrift = ::apache::thrift;
  namespace transport = ::apache::thrift::transport;
  namespace protocol = ::apache::thrift::protocol;

  ThriftFingerprintAuthnAdapter::ThriftFingerprintAuthnAdapter(const std::string& fingerprintAuthnServer,
							       int fingerprintAuthnPort) :
    socket(new transport::TSocket(fingerprintAuthnServer, fingerprintAuthnPort)),
    transport(new transport::TBufferedTransport(socket)),
    protocol(new protocol::TBinaryProtocol(transport)),
    client(protocol) {
    /* nothing */
  }

  ThriftFingerprintAuthnAdapter::~ThriftFingerprintAuthnAdapter() {
    /* nothing */ 
  }

  void
  ThriftFingerprintAuthnAdapter::authenticateImpl(iface::Credential& credential,
						  const std::string& fingerprint) {
    this->transport->open();
    this->client.fingerprintAuthn(credential, fingerprint);
    this->transport->close();
    return;
  }
}
