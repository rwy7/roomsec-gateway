/* -*- mode: c++ -*- */

#ifndef _ROOMSEC_GATEWAY_H_
#define _ROOMSEC_GATEWAY_H_

#include <boost/shared_ptr.hpp>

namespace roomsec {

  class AuthorityAdapter;
  class FingerprintAuthnAdapter;
  class DoorStateSensor;
  class FingerprintScanner;
  class Display;
  class Buzzer;
  class BlockSensor;

  /**
   * The central logic of the gateway system.  The gateway class
   * implements the control algorithms, and high level policy of the
   * gateway system.
   *
   * This is an abstract base class. Extending classes are responsible
   * for implementing the "guts" of the system.  This class is in
   * place to allow for multiple policy implementations.
   */
  class Gateway {
  public:

    virtual ~Gateway();

    /**
     * Start the gateway, and begin standard operation. This is a
     * blocking operation, and may not return.
     */    
    virtual void operator()() = 0;

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
      setAuthorityAdapter(boost::shared_ptr<AuthorityAdapter> authzAdapter) {
	this->authorityAdapter = authzAdapter;
	return *static_cast<BuilderT*>(this);
      }

      /**
       * Set the fingerprintauthn adapter used by the built Gateway.
       */
      BuilderT&
      setFingerprintAuthnAdapter(boost::shared_ptr<FingerprintAuthnAdapter> authnAdapter) {
	this->fingerprintAuthnAdapter = authnAdapter;
	return *static_cast<BuilderT*>(this);
      }

      /**
       * Set the fingerprint scanner device.
       */
      BuilderT&
      setFingerprintScanner(boost::shared_ptr<FingerprintScanner> fingerprintScanner) {
	this->fingerprintScanner = fingerprintScanner;
	return *static_cast<BuilderT*>(this);
      }

      BuilderT&
      setDoorStateSensor(boost::shared_ptr<DoorStateSensor> doorStateSensor) {
	this->doorStateSensor = doorStateSensor;
	return *static_cast<BuilderT*>(this);
      }

      BuilderT&
      setDisplay(boost::shared_ptr<Display> display) {
	this->display = display;
	return *static_cast<BuilderT*>(this);
      }
      
	BuilderT&
	setBuzzer(boost::shared_ptr<Buzzer> buzzer) {
		this->buzzer = buzzer;
		return *static_cast<BuilderT*>(this);
      }

	BuilderT&
	setBlockSensors(std::vector<BlockSensor*> sensors){
		this->blockSensors.clear();
		for(unsigned int i = 0; i < sensors.size(); i++)
			this->blockSensors.push_back(sensors[i]);
		return *static_cast<BuilderT*>(this);
	}

    protected:

      boost::shared_ptr<AuthorityAdapter> authorityAdapter;
      boost::shared_ptr<FingerprintAuthnAdapter> fingerprintAuthnAdapter;
      boost::shared_ptr<FingerprintScanner> fingerprintScanner;
      boost::shared_ptr<DoorStateSensor> doorStateSensor;
      boost::shared_ptr<Display> display;
      boost::shared_ptr<Buzzer> buzzer;
      std::vector<BlockSensor*> blockSensors;
    };

    template<typename B, typename G> friend class Gateway::Builder;


  protected:

    boost::shared_ptr<AuthorityAdapter> authzAdapter;
    boost::shared_ptr<FingerprintAuthnAdapter> authnAdapter;

    virtual void setAuthorityAdapter(boost::shared_ptr<AuthorityAdapter> authzAdapter);
    virtual void setFingerprintAuthnAdapter(boost::shared_ptr<FingerprintAuthnAdapter> authnAdapter);

  };
}

#endif /* _ROOMSEC_GATEWAY_H_ */
