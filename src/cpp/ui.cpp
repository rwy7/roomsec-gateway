#include "config.h"
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
    UiMessage(that.type, that.message)
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

  log4cxx::LoggerPtr Ui::logger(log4cxx::Logger::getLogger("roomsec.ui"));

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
    LOG4CXX_WARN(logger, "Not yet implemented");
    return retVal;
  }

  int
  Ui::message(boost::shared_ptr<const UiMessage> message) {
    /* Queue the message object for printing */
    LOG4CXX_WARN(logger, "Not yet implemented");
    
    switch(message->getType()) {
    default:
      display->clear();
      display->home();
      display->putStr(*message->getMessage());

      display->setBacklightColor(Display::red);
      buzzer->on();
      usleep(50000);
      buzzer->off();
      display->setBacklightColor(Display::blue);
    }

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
