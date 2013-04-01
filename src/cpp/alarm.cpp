#include <boost/thread.hpp>
#include <boost/signal.hpp>
#include "actor.h"
#include "alarm.h"

namespace roomsec {

  CountDown::CountDown(boost::chrono::milliseconds t)
    : t(t)
  {}

  CountDown::~CountDown()
  {}

  void
  CountDown::run() {
    boost::this_thread::sleep_for(this->t);
    this->act();
    return;
  }


  /*************/

  CountDownSignal::CountDownSignal(boost::chrono::milliseconds t)
    : CountDown(t)
  {}

  void
  CountDownSignal::act() {
    this->signal();
    return;
  }
}
