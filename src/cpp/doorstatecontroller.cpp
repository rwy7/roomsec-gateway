#include "config.h"
#include <thread>
#include <chrono>
#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>
#include "ui.h"
#include "uimessage.h"
#include "tailgateanalyzer.h"
#include "doorstatesensor.h"
#include "doorstatecontroller.h"

namespace roomsec {

  static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("roomsec.doorstatecontroller");
  //static log4cxx::LoggerPtr netLogger = log4cxx::Logger::getLogger("");


  DoorStateController::DoorStateController(std::string name,
					   boost::shared_ptr<DoorStateSensor> sensor,
					   boost::shared_ptr<TailgateAnalyzer> tailgateAnalyzer,
					   boost::shared_ptr<Ui> ui)
    : name(name), sensor(sensor), tailgateAnalyzer(tailgateAnalyzer), ui(ui), stop(false)
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
      PassageTriple result;

      switch(state) {
	
      case DoorState::closed:
	LOG4CXX_TRACE(logger, "State = closed");
	switch(nextState) {

	  // CLOSED -> CLOSED
	case DoorState::closed:
	  LOG4CXX_TRACE(logger, "Next State = closed");
	  break;

	  // CLOSED -> OPEN
	case DoorState::open:
	  LOG4CXX_TRACE(logger, "Next State = closed");
	  doorOpenTime = startTime;
	  ui->message(UiMessage::Type::info, "Door Open");
	  tailgateAnalyzer->beginSession();
	  // TODO: sleep / freeze for debouncing
	  break;
	}
	break;

      case DoorState::open:
	LOG4CXX_TRACE(logger, "State = open");
	switch(nextState) {

	  // OPEN -> CLOSED
	case DoorState::closed:
	  LOG4CXX_TRACE(logger, "Next State = closed");
	  if(alarmOn) {
	    ui->stopAlarm();
	    alarmOn = false;
	  }
	  tailgateAnalyzer->finishSession();
	  result = tailgateAnalyzer->getResults();
	  LOG4CXX_INFO(logger, "tailgate analysis: " << 
		       "in: "      << result.ingoing  << " " <<
		       "out: "     << result.outgoing << " " <<
		       "unknown: " << result.unknown);
	  break;

	  // OPEN -> OPEN
	case DoorState::open:
	  LOG4CXX_TRACE(logger, "Next State = open");
	  if (startTime - doorOpenTime > maxOpenTime) {
	    if (!alarmOn) {
	      // TODO: Start Alarm, NetLog.
	      LOG4CXX_INFO(logger, "Door alarm triggered.");
	      ui->startAlarm("Close door");
	      alarmOn = true;
	    }
	    tailgateAnalyzer->update();
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
