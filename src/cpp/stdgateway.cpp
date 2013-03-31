#include "config.h"
#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>

#include "actor.h"
#include "ui.h"
#include "gateway.h"
#include "stdgateway.h"

namespace roomsec {

  /*
   * Loggers
   */
  log4cxx::LoggerPtr
  StdGateway::logger(log4cxx::Logger::getLogger("roomsec.authority"));

  log4cxx::LoggerPtr
  StdGateway::netLogger(log4cxx::Logger::getLogger("roomsec.authority"));

  /*
   * Ctor / Dtor
   */

  StdGateway::StdGateway() {
  }

  /*
   * Threading
   */

  void
  StdGateway::init() {
    LOG4CXX_TRACE(logger, "Initializing StdGateway");
    
  }

  void
  StdGateway::run() {
    LOG4CXX_INFO(netLogger, "Gateway Up");
    // Ui ui();
    // ui.message(UiMessage::Type::warn, "Hello, World!");
    LOG4CXX_INFO(netLogger, "Gateway Down");
  }
}
