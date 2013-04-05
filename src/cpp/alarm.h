/* -*- Mode: c++ -*- */

#ifndef _ROOMSEC_ALARM_H_
#define _ROOMSEC_ALARM_H_


#include <atomic>
#include <boost/signal.hpp>
#include <boost/chrono.hpp>
#include "actor.h"

namespace roomsec {

  class CountDown : public Actor {
  public:
    CountDown(boost::chrono::milliseconds t);
    virtual ~CountDown();
    virtual void run();
    void cancelCountDown();

  private:
    virtual void act() = 0;
    boost::chrono::milliseconds t;
    std::atomic<bool> cancel;
  };

  class CountDownSignal : public CountDown {
  public:
    CountDownSignal(boost::chrono::milliseconds t);
    boost::signal<void ()> signal;

  private:
    virtual void act();
  };
    
}

#endif /* _ROOMSEC_ALARM_H_ */
