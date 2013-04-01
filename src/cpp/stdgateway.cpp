#include "config.h"
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include <log4cxx/logger.h>

#include "actor.h"
#include "ui.h"
#include "gateway.h"
#include "fingerprintcontroller.h"
#include "doorstatecontroller.h"
#include "stdgateway.h"

namespace roomsec {

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

    /* UI System */

    assert(this->display != NULL);
    assert(this->buzzer != NULL);
    boost::shared_ptr<Ui> ui(new Ui(this->display, this->buzzer));

    /* Door State Monitor and Controller */

    assert(this->doorStateSensor != NULL);
    boost::shared_ptr<DoorStateController>
      doorStateController(new DoorStateController(this->doorStateSensor));

    /* Fingerprint Scanner Monitor and Controller */

    /* TODO: */

    /* Tailgate Detection System */

    /* TODO: */

    // assert(this->fingerpintScanner != NULL);

    // boost::shared_ptr<FingerprintController> 
    //    fingerprintController(this->fingerprintScanner);

    // gateway->setFingerprintController(fingerprintController);

    boost::shared_ptr<StdGateway>
      gateway(new StdGateway(ui, doorStateController));

    return gateway;
  }

  /*
   * Ctor / Dtor
   */

  StdGateway::StdGateway(boost::shared_ptr<Ui> ui,
			 boost::shared_ptr<DoorStateController> doorStateController)
    : ui(ui), doorStateController(doorStateController)
  {

    doorStateController->sigDoorStateChange.connect([&] (DoorStateSensor::State state) {

	LOG4CXX_DEBUG(logger, "sigDoorStateChange called");

	if (state == DoorStateSensor::State::open) {
	  ui->message(UiMessage::Type::error, "Door Opened");
	}

	else if (state == DoorStateSensor::State::closed) {
	  ui->message(UiMessage::Type::error, "Door Closed");
	}
      });
  }

  /*
   * Threading
   */

  void
  StdGateway::init() {
    LOG4CXX_TRACE(logger, "Initializing StdGateway");
    
  }

  void
  StdGateway::begin() {
    LOG4CXX_INFO(netLogger, "Gateway Up");

    LOG4CXX_DEBUG(logger, "Starting Ui Actor");
    boost::thread uiThread = ui->start();

    LOG4CXX_DEBUG(logger, "Starting DoorStateController Actor");
    boost::thread doorStateControllerThread = doorStateController->start();
    
    LOG4CXX_DEBUG(logger, "Sleeping");
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10000));

    LOG4CXX_DEBUG(logger, "Waiting for threads to exit");
    uiThread.join();
    doorStateControllerThread.join();

    LOG4CXX_INFO(netLogger, "Gateway Down");
  }
}
