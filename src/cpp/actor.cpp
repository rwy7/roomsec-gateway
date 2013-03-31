#include "config.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "actor.h"

Actor::Actor() {
}

Actor::~Actor() {
}

boost::thread Actor::start() {
  return boost::thread(boost::bind(&Actor::run, this));
}
