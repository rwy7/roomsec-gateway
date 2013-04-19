#include "config.h"
#include <mutex>
#include "wiringPi/wiringPi.h"
#include "doorstatesensor.h"

namespace roomsec {
  DoorStateSensor::DoorStateSensor(int pin) : pin(pin) {
#ifdef ENABLE_GATEWAY
    pinMode(this->pin, INPUT);
    pullUpDnControl(this->pin, PUD_UP);
#endif
    this->update();
    return;
  }

  void DoorStateSensor::update() {
    int current = 0;
#ifdef ENABLE_GATEWAY
    current = digitalRead(this->pin);
#endif
    if (current == 1)
      this->state = State::open;
    else
      this->state = State::closed;
    return;
  }

  DoorStateSensor::~DoorStateSensor() {
    return;
  }

  DoorStateSensor::State DoorStateSensor::getDoorState() {
    std::lock_guard<std::mutex> guard(mutex);
    this->update();
    return this->state;
  }

}
