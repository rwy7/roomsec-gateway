/* -*- Mode: c++ -*- */
#ifndef _ROOMSEC_DOORSTATECONTROLLER_H_
#define _ROOMSEC_DOORSTATECONTROLLER_H_

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/signal.hpp>
#include "actor.h"
#include "doorstatesensor.h"

namespace roomsec {
  class DoorStateController : public Actor {
  public:
    DoorStateController(boost::shared_ptr<DoorStateSensor> sensor);
    virtual void run();

    boost::signal<void (DoorStateSensor::State)> sigDoorStateChange;
    
  private:
    boost::shared_ptr<DoorStateSensor> sensor;
    bool stop;

  };
}

#endif /* _ROOMSEC_DOORSTATECONTROLLER_H_ */
