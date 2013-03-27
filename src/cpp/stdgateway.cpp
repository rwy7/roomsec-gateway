#include "config.h"
#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>
#include "gateway.h"
#include "stdgateway.h"

namespace roomsec {
  log4cxx::LoggerPtr
  StdGateway::logger(log4cxx::Logger::getLogger("roomsec.authority"));

  log4cxx::LoggerPtr
  StdGateway::netLogger(log4cxx::Logger::getLogger("roomsec.authority"));

  StdGateway::StdGateway() {
  }

  void
  StdGateway::init() {
    LOG4CXX_TRACE(logger, "Initializing StdGateway");
    
  }

  void
  StdGateway::run() {
    LOG4CXX_INFO(netLogger, "Gateway Up");

    LOG4CXX_INFO(netLogger, "Gateway Down");
  }
}
