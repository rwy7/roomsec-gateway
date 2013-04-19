/* -*- Mode: c++ -*- */
#ifndef _ROOMSEC_DOORSTATECONTROLLER_H_
#define _ROOMSEC_DOORSTATECONTROLLER_H_

#include <boost/shared_ptr.hpp>

namespace roomsec {

  class DoorStateSensor;
  class TailgateAnalyzer;
  class Ui;

  class DoorStateController {

  public:

    DoorStateController(std::string name,
			boost::shared_ptr<DoorStateSensor> sensor,
			boost::shared_ptr<TailgateAnalyzer> tailgateAnalyzer,
			boost::shared_ptr<Ui> ui);

    void operator()();
    
  private:
    std::string name;
    boost::shared_ptr<DoorStateSensor> sensor;
    boost::shared_ptr<TailgateAnalyzer> tailgateAnalyzer;
    boost::shared_ptr<Ui> ui;
    bool stop;

  };
}

#endif /* _ROOMSEC_DOORSTATECONTROLLER_H_ */
