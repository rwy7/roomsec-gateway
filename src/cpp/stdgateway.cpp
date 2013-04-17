#include "config.h"
#include <thread>
#include <chrono>
#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include <log4cxx/logger.h>
#include "gen-cpp/authorize_types.h"
#include "ui.h"
#include "gateway.h"
#include "fingerprintcontroller.h"
#include "doorstatecontroller.h"
#include "authorityadapter.h"
#include "fingerprintscanner.h"
#include "fingerprintauthnadapter.h"
#include "stdgateway.h"

namespace roomsec {

  const std::string gatewayId = "gateway1";

  /*
   * Loggers
   */
  log4cxx::LoggerPtr
  StdGateway::logger(log4cxx::Logger::getLogger("roomsec.gateway"));

  log4cxx::LoggerPtr
  StdGateway::netLogger(log4cxx::Logger::getLogger("roomsec.net"));

  /*
   * Builder
   */

  boost::shared_ptr<StdGateway>
  StdGateway::Builder::build() {
    /* Networked Systems */
    // TODO: ASSERT

    /* UI System */

    assert(this->display != NULL);
    assert(this->buzzer != NULL);
    boost::shared_ptr<Ui> ui(new Ui(this->display, this->buzzer));

    /* Door State Monitor and Controller */

    assert(this->doorStateSensor != NULL);
    boost::shared_ptr<DoorStateController>
      doorStateController(new DoorStateController(this->doorStateSensor, ui));

    /* Fingerprint Scanner Monitor and Controller */

    assert(this->fingerprintScanner != NULL);
    boost::shared_ptr<FingerprintController>
      fingerprintController(new FingerprintController(this->fingerprintScanner));

    boost::shared_ptr<StdGateway>
      gateway(new StdGateway(ui,
			     doorStateController,
			     fingerprintController,
			     authorityAdapter,
			     fingerprintAuthnAdapter));

    return gateway;
  }

  void
  StdGateway::fingerprintScanned(boost::shared_ptr<Fingerprint> fingerprint) {

    LOG4CXX_DEBUG(logger, "Fingerprint Scanned");
    ui->message(UiMessage::Type::info, "Fingerprint Scanned");

    iface::Credential credential;
    fingerprintAuthnAdapter->authenticate(credential, fingerprint->serialize());

    if (credential.token == "" &&
	credential.userid == "") {
      ui->message(UiMessage::Type::error, "User Unrecognized");
      LOG4CXX_INFO(netLogger, "roomsec." << gatewayId << ".userauth.fail");
    }
    else {
      ui->message(UiMessage::Type::warning, "User: "+ credential.userid);

      LOG4CXX_INFO(netLogger,
		   "roomsec." << gatewayId << 
		   ".userauth.pass." << credential.userid);
    }
    return;
  }


  /*
   * Ctor / Dtor
   */

  StdGateway::StdGateway(boost::shared_ptr<Ui> ui,
			 boost::shared_ptr<DoorStateController> doorStateController,
			 boost::shared_ptr<FingerprintController> fingerprintController,
			 boost::shared_ptr<AuthorityAdapter> authorityAdapter,
			 boost::shared_ptr<FingerprintAuthnAdapter> fingerprintAuthnAdapter)
    : ui(ui),
      doorStateController(doorStateController),
      fingerprintController(fingerprintController),
      authorityAdapter(authorityAdapter),
      fingerprintAuthnAdapter(fingerprintAuthnAdapter)
  {
    fingerprintController
      ->fingerprintScanned
      .connect(boost::bind(&StdGateway::fingerprintScanned, this, _1));
  }


  void
  StdGateway::operator()() {
    LOG4CXX_TRACE(logger, "Initializing StdGateway");
    LOG4CXX_INFO(netLogger, "roomsec." << gatewayId << ".online");

    LOG4CXX_DEBUG(logger, "Starting Ui Actor");
    std::thread uiThread(std::ref(*ui));
    ui->message(UiMessage::Type::warning, "Initializing");

    LOG4CXX_DEBUG(logger, "Starting DoorStateController Actor");
    std::thread doorStateControllerThread(std::ref(*doorStateController));

    LOG4CXX_DEBUG(logger, "Starting FingerprintController Actor");
    std::thread fingerprintControllerThread(std::ref(*fingerprintController));

    LOG4CXX_DEBUG(logger, "Sleeping");
    std::this_thread::sleep_for(std::chrono::milliseconds(100000));

    LOG4CXX_DEBUG(logger, "Waiting for threads to exit");
    uiThread.join();
    doorStateControllerThread.join();
    fingerprintControllerThread.join();

    LOG4CXX_INFO(netLogger, "roomsec." << gatewayId << ".online");
    return;
  }
}
