#include "config.h"
#include <thread>
#include <chrono>
#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>
#include "ui.h"
#include "uimessage.h"
#include "doorstatesensor.h"
#include "doorstatecontroller.h"

namespace roomsec {

  static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("roomsec.doorstatecontroller");
  //static log4cxx::LoggerPtr netLogger = log4cxx::Logger::getLogger("");


  DoorStateController::DoorStateController(boost::shared_ptr<DoorStateSensor> sensor,
					   boost::shared_ptr<Ui> ui)
    : sensor(sensor), ui(ui), stop(false)
  {
    LOG4CXX_TRACE(logger, "Constructing DoorStateController");
  }


  /*
   * The doorstate controller polls every 10 or so milliseconds.  If
   * the door is left open, it sounds an alarm.
   */

  void
  DoorStateController::operator()()
  {
    LOG4CXX_DEBUG(logger, "DoorStateController running");
    LOG4CXX_TRACE(logger, "Initializing state");

    DoorState state = sensor->getDoorState();
    bool alarmOn = false;

    std::chrono::system_clock::time_point
      doorOpenTime = std::chrono::system_clock::now();

    const std::chrono::seconds maxOpenTime(10);
    const std::chrono::milliseconds period(10);
    
    while(!this->stop) {

      LOG4CXX_TRACE(logger, "Begin cycle");

      std::chrono::system_clock::time_point startTime = 
	std::chrono::system_clock::now();

      DoorState nextState = sensor->getDoorState();

      switch(state) {

      case DoorState::closed:
	LOG4CXX_TRACE(logger, "State = closed");
	switch(nextState) {
	case DoorState::closed:
	  LOG4CXX_TRACE(logger, "Next State = closed");
	  break;

	case DoorState::open:
	  LOG4CXX_TRACE(logger, "Next State = closed");
	  doorOpenTime = startTime;
	  ui->message(UiMessage::Type::info, "Door Open");
	  break;
	}
	break;

      case DoorState::open:
	LOG4CXX_TRACE(logger, "State = open");
	switch(nextState) {
	case DoorState::closed:
	  LOG4CXX_TRACE(logger, "Next State = closed");
	  if(alarmOn) {
	    ui->alarmOff();
	    alarmOn = false;
	    break;

	case DoorState::open:
	  LOG4CXX_TRACE(logger, "Next State = open");
	  if (startTime - doorOpenTime > maxOpenTime) {
	    if (!alarmOn) {
	      // TODO: Start Alarm, NetLog.
	      LOG4CXX_INFO(logger, "Door alarm triggered.");
	      ui->alarmOn("Close door");
	      alarmOn = true;
	    }
	  }
	  break;
	}
	break;
      }
      
      state = nextState;
      std::this_thread::sleep_until(startTime + period);
    }

    LOG4CXX_DEBUG(logger, "DoorStateController stopping");
  }

}


/*
    LOG4CXX_DEBUG(logger, "sigDoorStateChange called");

    if (state == DoorStateSensor::State::open) {
      ui->message(UiMessage::Type::warning, "Door Opened");
      LOG4CXX_INFO(netLogger, "roomsec." << gatewayId << ".door.open");
      doorAlarmCountDown.cancelCountDown();
      doorAlarmThread = doorAlarmCountDown.start();
    }

    else if (state == DoorStateSensor::State::closed) {
      ui->message(UiMessage::Type::warning, "Door Closed");
      LOG4CXX_INFO(netLogger, "roomsec." << gatewayId << ".door.close");

      doorAlarmCountDown.cancelCountDown();
      ui->stopAlarm();
    }
    return;

  void StdGateway::signalDoorAlarm() {
    LOG4CXX_DEBUG(logger, "Starting DoorStateController Actor");
    LOG4CXX_INFO(netLogger, "roomsec." << gatewayId << ".alarm.door");
    ui->startAlarm("Close Door");
  }

*/
