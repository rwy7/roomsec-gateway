/* -*- Mode: c++ -*- */

#ifndef _ROOMSEC_ACTOR_H_
#define _ROOMSEC_ACTOR_H_

#include <boost/thread.hpp>

namespace roomsec {
  class Actor {
  public:
    Actor();
    virtual ~Actor();
    virtual void run() = 0;
    boost::thread start();
  };
}

#endif /* _ROOMSEC_ACTOR_H_ */
