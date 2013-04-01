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

  UiMessage::UiMessage(Type type, boost::shared_ptr<const std::string> message) :
    type(type), message(message) 
  {
    LOG4CXX_TRACE(logger, "UiMessage created: " << *this->message);
  }

  UiMessage::UiMessage(const UiMessage & that) :
    type(that.getType()), message(that.getMessage())
  {
  }

  UiMessage::Type
  UiMessage::getType() const {
    return this->type;
  }

  boost::shared_ptr<const std::string>
  UiMessage::getMessage() const {
    return this->message;
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

    while(!this->stop) {

      LOG4CXX_DEBUG(logger, "Waiting for message");
      boost::shared_ptr<const UiMessage> message(this->messageQueue.front_pop());

      LOG4CXX_DEBUG(logger, "Writing Message: " << *message->getMessage());
      display->clear();
      display->home();

      switch(message->getType()) {

      case UiMessage::Type::info:
	display->setBacklightColor(Display::blue);
	display->putStr(*message->getMessage());
	boost::this_thread::sleep_for(boost::chrono::milliseconds(messageTime));
	break;

      case UiMessage::Type::error:
	display->setBacklightColor(Display::red);
	display->putStr(*message->getMessage());
	buzzer->on();
	boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
	buzzer->off();
	boost::this_thread::sleep_for(boost::chrono::milliseconds(messageTime - 500));
	break;

      case UiMessage::Type::prompt:
      case UiMessage::Type::warning:
	display->setBacklightColor(Display::green);
	display->putStr(*message->getMessage());
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
  Ui::message(UiMessage const& that) {
    /* Copy the UiMessage, and queue the copy for display.
     */
    boost::shared_ptr<const UiMessage> copy(new UiMessage(that));
    int retVal = message(copy);
    return retVal;
  }

  int
  Ui::message(UiMessage::Type type, std::string const& str) {
    /* Dynamically copy the message string.  If no copy is desired,
     * pass in a shared pointer or message object.  Construct and
     * queue the new message.  If the queue is full or allocation
     * fails, return -1.
     */
    int retVal = 0;
    boost::shared_ptr<const std::string> messageString(new std::string(str));
    boost::shared_ptr<const UiMessage> message(new UiMessage(type, messageString));
    retVal = Ui::message(message);
    return retVal;
  }

  int
  Ui::message(boost::shared_ptr<const UiMessage> message) {
    /* Queue the message object for printing */
    LOG4CXX_DEBUG(logger, "Queueing message: " << *message->getMessage());
    messageQueue.push(message);
    return 0;
  }

  /** TODO: Implement */
  int
  Ui::message(UiMessage::Type t) {
    LOG4CXX_WARN(logger, "Not yet implemented");
    return -1;
  }

  int
  Ui::message(std::string const & str) {
    LOG4CXX_WARN(logger, "Not yet implemented");
    return -1;
  }
}
