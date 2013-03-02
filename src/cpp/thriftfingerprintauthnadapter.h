#ifndef _ROOMSEC_THRIFTFINGERPRINTAUTHNADAPTER_H_
#define _ROOMSEC_THRIFTFINGERPRINTAUTHNADAPTER_H_

#include "gen-cpp/FingerprintAuthn.h"
#include "fingerprintauthnadapter.h"

namespace roomsec {

  class ThriftFingerprintAuthnAdapter: public FingerprintAuthnAdapter {
  public:
    ThriftFingerprintAuthnAdapter(const std::string& fingerprintAuthnServer,
				  int fingerprintAuthnPort);
    virtual ~ThriftFingerprintAuthnAdapter();

  protected:
    boost::shared_ptr<apache::thrift::transport::TSocket> socket;
    boost::shared_ptr<apache::thrift::transport::TTransport> transport;
    boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol;
    iface::FingerprintAuthnClient client;

  private:
    void authenticateImpl(iface::Credential& credential,
			  const std::string& fingerprint);
  };
}

#endif /* _ROOMSEC_THRIFTFINGERPRINTAUTHNADAPTER_H_ */
