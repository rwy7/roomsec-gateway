#include "config.h"
#include "log4cxx/logger.h"
#include "uimessage.h"

namespace roomsec {
  
  static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("roomsec.ui"));

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
}
