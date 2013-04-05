#include <boost/thread.hpp>
#include <boost/signal.hpp>
#include "actor.h"
#include "alarm.h"

namespace roomsec {

  CountDown::CountDown(boost::chrono::milliseconds t)
    : t(t), cancel(false)
  {}

  CountDown::~CountDown()
  {}

  void
  CountDown::run() {
    boost::this_thread::sleep_for(this->t);
    boost::this_thread::interruption_point();
    if (!this->cancel) {
      this->act();
    }
    return;
  }

  void
  CountDown::cancelCountDown() {
    this->cancel = true;
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
