/* -*- Mode: c++ -*- */
#ifndef _ROOMSEC_DOORSTATECONTROLLER_H_
#define _ROOMSEC_DOORSTATECONTROLLER_H_

#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>
#include "doorstatesensor.h"

namespace roomsec {

  class DoorStateSensor;

  class DoorStateController {

  public:

    DoorStateController(boost::shared_ptr<DoorStateSensor> sensor);
    void operator()();
    
  private:

    static log4cxx::LoggerPtr logger;
    boost::shared_ptr<DoorStateSensor> sensor;
    bool stop;

  };
}

#endif /* _ROOMSEC_DOORSTATECONTROLLER_H_ */
