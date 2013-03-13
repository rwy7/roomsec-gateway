#include "gateway.h"

namespace roomsec {

  Gateway::~Gateway() {
  }

  void Gateway::start() {
    this->init();
    this->run();
  }
  
}
