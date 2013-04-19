/* -*- Mode: c++ -*- */
#ifndef _ROOMSEC_STDGATEWAY_H_
#define _ROOMSEC_STDGATEWAY_H_

#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>
#include "gateway.h"

namespace roomsec {

  class DoorStateController;
  class Ui;
  class FingerprintController;
  class Fingerprint;

  /**
   * The standard operations gateway controller.  This controller
   * should be used when the system is in standard autonomous
   * operations.  This gateway implements the neccesary high level
   * logic to ensure that all subsystem compose a secure and effective
   * gateway system.
   */
  class StdGateway : public Gateway {

  public:

    class Builder : public Gateway::Builder<StdGateway::Builder, StdGateway> {
    public:

      virtual boost::shared_ptr<StdGateway> build();

    };

    friend class StdGateway::Builder;
    virtual void operator()();

  protected:

    StdGateway(std::string name,
	       boost::shared_ptr<DoorStateController> doorStateController,
	       boost::shared_ptr<FingerprintController> fingerprintController,
	       boost::shared_ptr<AuthorityAdapter> authorityAdapter,
	       boost::shared_ptr<FingerprintAuthnAdapter> fingerprintAuthnAdapter,
	       boost::shared_ptr<Ui> ui);

  private:

    static log4cxx::LoggerPtr logger;
    static log4cxx::LoggerPtr netLogger;

    std::string name;
    boost::shared_ptr<DoorStateController> doorStateController;
    boost::shared_ptr<FingerprintController> fingerprintController;
    boost::shared_ptr<AuthorityAdapter> authorityAdapter;
    boost::shared_ptr<FingerprintAuthnAdapter> fingerprintAuthnAdapter;
    boost::shared_ptr<Ui> ui;

  };
}

#endif /* _ROOMSEC_STDGATEWAY_H_ */
