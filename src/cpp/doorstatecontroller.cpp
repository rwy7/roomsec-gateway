#include "config.h"

#include <boost/shared_ptr.hpp>
//#include <boost/bind.hpp>
#include <boost/signal.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <log4cxx/logger.h>
#include "actor.h"
#include "doorstatesensor.h"
#include "doorstatecontroller.h"

namespace roomsec {

  log4cxx::LoggerPtr DoorStateController::logger =
    log4cxx::Logger::getLogger("roomsec.doorstatecontroller");

  DoorStateController::DoorStateController(boost::shared_ptr<DoorStateSensor> sensor)
    : sensor(sensor), stop(false)
  {
    LOG4CXX_TRACE(logger, "DoorStateController Constructed");
  }

  void
  DoorStateController::run()
  {
    LOG4CXX_DEBUG(logger, "DoorStateController running");

    DoorStateSensor::State oldState = sensor->getDoorState();

    while(!this->stop) {

      DoorStateSensor::State nextState = sensor->getDoorState();
      if (nextState != oldState) {
	LOG4CXX_DEBUG(logger, "Door State changed");
	this->sigDoorStateChange(nextState);
	oldState = nextState;
      }
      LOG4CXX_TRACE(logger, "Looping");

      boost::this_thread::sleep_for(boost::chrono::milliseconds(50));
    }
    LOG4CXX_DEBUG(logger, "DoorStateController stopping");
  }

}
