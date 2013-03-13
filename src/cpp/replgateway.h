#ifndef _ROOMSEC_REPLGATEWAY_H_
#define _ROOMSEC_REPLGATEWAY_H_

#include <boost/shared_ptr.hpp>
#include "gateway.h"

namespace roomsec {

  class AuthorityAdapter;
  class FingerprintAuthnAdapter;

  class ReplGateway : public Gateway {

  public:
    ReplGateway();
    virtual ~ReplGateway();

    class Builder : public Gateway::Builder<ReplGateway::Builder, ReplGateway>{

    public:

      virtual boost::shared_ptr<ReplGateway> build();

      virtual ReplGateway::Builder&
      authorityAdapter(boost::shared_ptr<AuthorityAdapter> authzAdapter);

      virtual ReplGateway::Builder&
      fingerprintAuthnAdapter(boost::shared_ptr<FingerprintAuthnAdapter> authnAdapter);

    protected:

      boost::shared_ptr<AuthorityAdapter> authzAdapter;
      boost::shared_ptr<FingerprintAuthnAdapter> authnAdapter;

    };

    friend class Builder;

  protected:

    void setAuthorityAdapter(boost::shared_ptr<AuthorityAdapter> authzAdapter);

    void setFingerprintAuthnAdapter(boost::shared_ptr<FingerprintAuthnAdapter> authnAdapter);

  private:

    virtual void init();

    virtual void run();

    void start_repl();

    boost::shared_ptr<AuthorityAdapter> authzAdapter;
    boost::shared_ptr<FingerprintAuthnAdapter> authnAdapter;

  };
}

#endif /* _ROOMSEC_REPLGATEWAY_H_ */
