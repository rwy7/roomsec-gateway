#include "config.h"
#include "wiringPi/wiringPi.h"
#include "doorstatesensor.h"

namespace roomsec {
  DoorStateSensor::DoorStateSensor(int pin) : pin(pin) {
#ifdef ENABLE_GATEWAY
    pinMode(this->pin, INPUT);
    pullUpDnControl(this->pin, PUD_DOWN);
#endif
    this->update();
    return;
  }

  void DoorStateSensor::update() {
    int current = 0;
#ifdef ENABLE_GATEWAY
    current = digitalRead(this->pin);
#endif
    if (current == 0)
      this->state = State::open;
    else
      this->state = State::closed;
    return;
  }

  DoorStateSensor::~DoorStateSensor() {
    return;
  }

  DoorStateSensor::State DoorStateSensor::getDoorState() {
    this->update();
    return this->state;
  }

  int DoorStateSensor::waitForChange(void (*callback)(void)) {
#ifdef ENABLE_GATEWAY
    return (wiringPiISR(this->pin, INT_EDGE_BOTH, callback));
#else
    return (0);
#endif
  }
}
