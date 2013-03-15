#ifndef _ROOMSEC_REPLGATEWAY_H_
#define _ROOMSEC_REPLGATEWAY_H_

#include <boost/shared_ptr.hpp>
#include "gateway.h"

namespace roomsec {

  class AuthorityAdapter;
  class FingerprintAuthnAdapter;

  /**
   * Implements a gateway controlled with a REPL interface.  This
   * class allows a human user to control the high level logic of the
   * system.  This is intended for demonstration and testing.
   */
  class ReplGateway : public Gateway {

  public:
    
    virtual ~ReplGateway();

    /**
     * This builder class contructs a Repl gateway controller.
     */
    class Builder : public Gateway::Builder<ReplGateway::Builder, ReplGateway>{

    public:

      virtual boost::shared_ptr<ReplGateway> build();

      virtual ReplGateway::Builder&
      authorityAdapter(boost::shared_ptr<AuthorityAdapter> authzAdapter);

      virtual ReplGateway::Builder&
      fingerprintAuthnAdapter(boost::shared_ptr<FingerprintAuthnAdapter> authnAdapter);

    protected:

      /**
       * This class is protected.  Only a friend class, such as the
       * ReplGateway::Builder can construct a ReplGateway object.
       * This is to ensure that the user always works through the
       * builder's interface, rather than attempt to construct a
       * ReplGateway directly.
       */
      ReplGateway();
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
