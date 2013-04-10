#include "config.h"
#include <thread>
#include <chrono>
#include <mutex>
#include <boost/shared_ptr.hpp>
#include "buzzer.h"
#include "display.h"
#include "ui.h"

namespace roomsec {
  
  /********************
   * UiMessage
   ********************/
  
  log4cxx::LoggerPtr UiMessage::logger(log4cxx::Logger::getLogger("roomsec.ui"));

  UiMessage::UiMessage(Type type, std::string message)
    : type(type), message(message) 
  {
    LOG4CXX_TRACE(logger, "UiMessage created: " << message);
  }

  UiMessage::UiMessage(UiMessage const& that) :
    type(that.getType()), message(that.getMessage())
  {
  }

  UiMessage::Type
  UiMessage::getType() const {
    return type;
  }

  std::string
  UiMessage::getMessage() const {
    return message;
  }

  /********************
   * Ui
   ********************/

  log4cxx::LoggerPtr Ui::logger(log4cxx::Logger::getLogger("roomsec.ui"));

  Ui::Ui(boost::shared_ptr<Display> display,
	 boost::shared_ptr<Buzzer> buzzer)
    : alarmMessage(""), alarmOn(false), display(display), 
      buzzer(buzzer), stop(false)
  {
    display->setBacklightColor(Display::green);
  }

  void
  Ui::run() {
    LOG4CXX_DEBUG(logger, "Ui running");

    const int messageTime = 2000; // milliseconds;

    while(!stop) {

      LOG4CXX_DEBUG(logger, "Waiting for message");
      UiMessage message = messageQueue.front_pop();

      LOG4CXX_DEBUG(logger, "Writing Message: " << message.getMessage());
      display->clear();
      display->home();

      switch(message.getType()) {

      case UiMessage::Type::info:
	display->setBacklightColor(Display::blue);
	display->putStr(message.getMessage());
	std::this_thread::sleep_for(std::chrono::milliseconds(messageTime));
	break;

      case UiMessage::Type::error:
	display->setBacklightColor(Display::red);
	display->putStr(message.getMessage());
	buzzer->on();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	buzzer->off();
	std::this_thread::sleep_for(std::chrono::milliseconds(messageTime - 500));
	break;

      case UiMessage::Type::prompt:
      case UiMessage::Type::warning:
	display->setBacklightColor(Display::green);
	display->putStr(message.getMessage());
	buzzer->on();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	buzzer->off();
	std::this_thread::sleep_for(std::chrono::milliseconds(messageTime - 500));
	break;

      case UiMessage::Type::alarm:
	/* Do nothing, the steady state defaults have changed. */
	break;

      default:
	display->putStr("Unrecognized Message Type!");
	buzzer->on();
	std::this_thread::sleep_for(std::chrono::milliseconds(messageTime));
	buzzer->off();
      }

      this->enterDefaultState();

      /*  Default the message to blue.  Check after clearing the screen if the
       *  alarm is on.  Since checking the alarm requires mutex access, this is
       *  necessary.*/

      // std::this_thread::interruption_point();
      // boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    }

    LOG4CXX_DEBUG(logger, "Ui stopping");
    return;
  }

  /** Messaging Functions */

  int
    Ui::message(UiMessage const& msg) {
      LOG4CXX_DEBUG(logger, "Queueing message: " << msg.getMessage());
      int retVal = 0;
      messageQueue.push(msg);
      return retVal;
    }

  int
    Ui::startAlarm(std::string const& message) {
      int retVal = 0;
      std::unique_lock<std::mutex> lock(this->mutex);
      this->alarmOn = true;
      this->alarmMessage = message;
      LOG4CXX_DEBUG(logger, "Starting alarm: " << this->alarmMessage);
      this->messageQueue.push(UiMessage(UiMessage::Type::alarm, ""));
      return retVal;
    }

  int
    Ui::stopAlarm() {
      std::unique_lock<std::mutex> lock(this->mutex);
      this->alarmOn = false;
      this->alarmMessage = "";
      this->messageQueue.push(UiMessage(UiMessage::Type::alarm, ""));
      return 0;
    }

  void Ui::enterDefaultState() {
    std::unique_lock<std::mutex> lock(this->mutex);
    if (alarmOn) {
      buzzer->on();
      display->clear();
      display->home();
      display->setBacklightColor(Display::red);
      display->putStr(this->alarmMessage);
    }
    else {
      display->clear();
      display->home();
      display->setBacklightColor(Display::blue);
    }
    return;
  }

  int
    Ui::message(UiMessage::Type type, std::string const& str) {
      UiMessage msg(type, str);
      return message(msg);
    }
}
