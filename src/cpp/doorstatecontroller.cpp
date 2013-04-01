#include "config.h"

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/signal.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "actor.h"
#include "doorstatesensor.h"

#include "doorstatecontroller.h"

namespace roomsec {

  DoorStateController::DoorStateController(boost::shared_ptr<DoorStateSensor> sensor)
    : sensor(sensor), stop(false)
  {

  }

  void
  DoorStateController::run()
  {
    DoorStateSensor::State oldState = sensor->getDoorState();
    while(!this->stop) {
      DoorStateSensor::State nextState = sensor->getDoorState();
      if (nextState != oldState) {
	this->sigDoorStateChange(nextState);
	oldState = nextState;
      }

      boost::this_thread::sleep_for(boost::chrono::milliseconds(50));
    }
  }

}
