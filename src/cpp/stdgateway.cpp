#include "config.h"
#include <thread>
#include <chrono>
#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>

#include "ui.h"
#include "gateway.h"
#include "fingerprintcontroller.h"
#include "doorstatecontroller.h"
#include "tailgateanalyzer.h"
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
    /* Networked Systems */
    // TODO: ASSERTs

    /* UI System */

    assert(this->display != NULL);
    assert(this->buzzer != NULL);
    boost::shared_ptr<Ui> ui(new Ui(this->display, this->buzzer));


    /* Tailgate Analyzer */
    boost::shared_ptr<TailgateAnalyzer> tailgateAnalyzer(new TailgateAnalyzer(blockSensors));

    /* Door State Monitor and Controller */


    assert(this->doorStateSensor != NULL);
    boost::shared_ptr<DoorStateController>
      doorStateController(new DoorStateController(name,
						  doorStateSensor,
						  tailgateAnalyzer,
						  ui));

    /* Fingerprint Scanner Monitor and Controller */

    assert(this->fingerprintScanner != NULL);
    boost::shared_ptr<FingerprintController>
      fingerprintController(new FingerprintController(name,
						      fingerprintScanner,
						      authorityAdapter,
						      fingerprintAuthnAdapter,
						      lock,
						      doorStateSensor,
						      ui));

    boost::shared_ptr<StdGateway>
      gateway(new StdGateway(name,
			     doorStateController,
			     fingerprintController,
			     authorityAdapter,
			     fingerprintAuthnAdapter,
			     ui));

    return gateway;
  }


  /*
   * Ctor / Dtor
   */

  StdGateway::StdGateway(std::string name,
			 boost::shared_ptr<DoorStateController> doorStateController,
			 boost::shared_ptr<FingerprintController> fingerprintController,
			 boost::shared_ptr<AuthorityAdapter> authorityAdapter,
			 boost::shared_ptr<FingerprintAuthnAdapter> fingerprintAuthnAdapter,
			 boost::shared_ptr<Ui> ui)
    : name(name),
      doorStateController(doorStateController),
      fingerprintController(fingerprintController),
      authorityAdapter(authorityAdapter),
      fingerprintAuthnAdapter(fingerprintAuthnAdapter),
      ui(ui)
  {
  }


  /*
   * Running Code
   */

  void
  StdGateway::operator()() {
    LOG4CXX_TRACE(logger, "Initializing StdGateway");
    LOG4CXX_INFO(netLogger, "roomsec." << name << ".online");

    LOG4CXX_DEBUG(logger, "Starting Ui Actor");
    std::thread uiThread(std::ref(*ui));
    ui->message(UiMessage::Type::warning, "Initializing");

    LOG4CXX_DEBUG(logger, "Starting DoorStateController Actor");
    std::thread doorStateControllerThread(std::ref(*doorStateController));

    LOG4CXX_DEBUG(logger, "Starting FingerprintController Actor");
    std::thread fingerprintControllerThread(std::ref(*fingerprintController));

    LOG4CXX_DEBUG(logger, "Waiting for threads to exit");
    uiThread.join();
    doorStateControllerThread.join();
    fingerprintControllerThread.join();

    LOG4CXX_INFO(netLogger, "roomsec." << name << ".online");
    return;
  }
}
