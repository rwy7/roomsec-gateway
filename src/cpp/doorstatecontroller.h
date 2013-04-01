/* -*- Mode: c++ -*- */
#ifndef _ROOMSEC_DOORSTATECONTROLLER_H_
#define _ROOMSEC_DOORSTATECONTROLLER_H_

#include <boost/shared_ptr.hpp>
//#include <boost/bind.hpp>
#include <boost/signals2.hpp>
#include <log4cxx/logger.h>

#include "actor.h"
#include "doorstatesensor.h"

namespace roomsec {
  class DoorStateController : public Actor {
  public:
    DoorStateController(boost::shared_ptr<DoorStateSensor> sensor);
    virtual void run();

    boost::signals2::signal<void (DoorStateSensor::State)> sigDoorStateChange;
    
  private:
    static log4cxx::LoggerPtr logger;
    boost::shared_ptr<DoorStateSensor> sensor;
    bool stop;

  };
}

#endif /* _ROOMSEC_DOORSTATECONTROLLER_H_ */
