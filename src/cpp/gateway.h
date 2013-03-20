/* -*- mode: c++; fill-column: 79 -*- */

#ifndef _ROOMSEC_GATEWAY_H_
#define _ROOMSEC_GATEWAY_H_

#include <boost/shared_ptr.hpp>

namespace roomsec {

  class AuthorityAdapter;
  class FingerprintAuthnAdapter;

  /**
   * @brief The central logic of the gateway system.  The gateway
   * class implements the control algorithms, and high level policy of
   * the gateway system.
   */
  class Gateway {
  public:

    virtual ~Gateway();

    /**
     * Start the gateway, and begin standard operation. This is a
     * blocking operation, that may not return.
     *
     * This function is an algorithm template, and is responsible for
     * calling init and run in derived classes.
     */    
    void start();

    /**
     * @class Builder
     * Responsible for constructing gateway objects.  Gateway objects
     * must be constructed through a builder class.
     *
     * Establishing the basic configuration of a gatway object is
     * complex, and the builder class helps alleviate the complication
     * of stepwise system configuration.
     *
     * Builder classes will provide a fluent interface for
     * constructing objects.  Each function exported by the builder
     * interface represents a configuration for the gateway object.
     *
     * Each derived gateway class should implement their own builder
     * class.
     */
    template<typename BuilderT, typename GatewayT>
    class Builder {

    public:
      virtual ~Builder() {};

      /**
       * Construct and return the gateway object.  This should be the
       * last function that is called.
       */
      virtual boost::shared_ptr<GatewayT> build() = 0;

      /**
       * Set the authority adapter used by the built Gateway.
       */
      BuilderT&
      authorityAdapter(boost::shared_ptr<AuthorityAdapter> authzAdapter) {
	this->authzAdapter = authzAdapter;
	return *static_cast<BuilderT*>(this);
      }

      /**
       * Set the fingerprintauthn adapter used by the built Gateway.
       */
      BuilderT&
      fingerprintAuthnAdapter(boost::shared_ptr<FingerprintAuthnAdapter> authnAdapter) {
	this->authnAdapter = authnAdapter;
	return *static_cast<BuilderT*>(this);
      }
      
    protected:

      boost::shared_ptr<AuthorityAdapter> authzAdapter;
      boost::shared_ptr<FingerprintAuthnAdapter> authnAdapter;
      
    };

    //friend class Gateway::Builder<>;
    
  private:

    /**
     * Initialize subsystems.  This method provides a mechanism for
     * derived gatway classes to have their initialization code
     * called.  This method is called from the start function
     * implemented in this Gateway base class.
     */
    virtual void init () = 0;

    /**
     * Run the gatway control module.  Begin continuous standard
     * operation.  This is a blocking operation that may not return.
     * This function must be implemented by derived classes.
     *
     * Derived classes implement the policy of the system.  Differing
     * managers may be implemented.
     */
    virtual void run() = 0;
  };
}

#endif /* _ROOMSEC_GATEWAY_H_ */
