/* -*- Mode: c++ -*- */
#ifndef _ROOMSEC_DOORSTATECONTROLLER_H_
#define _ROOMSEC_DOORSTATECONTROLLER_H_

#include <boost/shared_ptr.hpp>

namespace roomsec {

  class DoorStateSensor;

  class DoorStateController {

  public:

    DoorStateController(boost::shared_ptr<DoorStateSensor> sensor);
    void operator()();
    
  private:

    boost::shared_ptr<DoorStateSensor> sensor;
    bool stop;

  };
}

#endif /* _ROOMSEC_DOORSTATECONTROLLER_H_ */
