#include "config.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
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

  namespace b = ::boost;

  log4cxx::LoggerPtr Ui::logger(log4cxx::Logger::getLogger("roomsec.ui"));

  Ui::Ui(b::shared_ptr<Display> display, b::shared_ptr<Buzzer> buzzer) :
    display(display), buzzer(buzzer), stop(false)
  {
    display->setBacklightColor(Display::green);
  }

  void
  Ui::run() {
    LOG4CXX_DEBUG(logger, "Ui running");

    const int messageTime = 5000; // milliseconds;

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
	boost::this_thread::sleep_for(boost::chrono::milliseconds(messageTime));
	break;

      case UiMessage::Type::error:
	display->setBacklightColor(Display::red);
	display->putStr(message.getMessage());
	buzzer->on();
	boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
	buzzer->off();
	boost::this_thread::sleep_for(boost::chrono::milliseconds(messageTime - 500));
	break;

      case UiMessage::Type::prompt:
      case UiMessage::Type::warning:
	display->setBacklightColor(Display::green);
	display->putStr(message.getMessage());
	buzzer->on();
	boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
	buzzer->off();
	boost::this_thread::sleep_for(boost::chrono::milliseconds(messageTime - 500));
	break;

      default:
	display->putStr("Unrecognized Message Type!");
	buzzer->on();
	boost::this_thread::sleep_for(boost::chrono::milliseconds(messageTime));
	buzzer->off();
      }

      /* Return to default, wait 0.5 seconds before processing next
       * message.
       */
      display->clear();
      display->home();
      display->setBacklightColor(Display::blue);
      boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
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
  Ui::message(UiMessage::Type type, std::string const& str) {
    UiMessage msg(type, str);
    return message(msg);
  }
}
