/* -*- Mode: c++ -*- */

#ifndef _ROOMSEC_ACTOR_H_
#define _ROOMSEC_ACTOR_H_

#include <thread>

namespace roomsec {
  class Actor {
  public:
    Actor();
    virtual ~Actor();
    virtual void run() = 0;
    std::thread start();
  };
}

#endif /* _ROOMSEC_ACTOR_H_ */
