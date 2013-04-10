#include "config.h"
#include <thread>
#include "actor.h"

namespace roomsec {

  Actor::Actor() {
  }

  Actor::~Actor() {
  }

  std::thread Actor::start() {
    return std::thread{&Actor::run, this};
  }
}
