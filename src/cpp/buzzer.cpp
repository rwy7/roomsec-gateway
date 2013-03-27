#include "config.h"
#include <wiringPi/wiringPi.h>
#include "buzzer.h"

namespace roomsec {

  Buzzer::Buzzer(int pin) : pin(pin) {
#ifdef ENABLE_GATEWAY
    pinMode(this->pin, OUTPUT);
    digitalWrite(this->pin, 0);
#endif
    this->state = State::off;
    return;
  }

  Buzzer::~Buzzer() {
    return;
  }

  bool Buzzer::on() {
    if (this->state == Buzzer::State::off) {
#ifdef ENABLE_GATEWAY
      digitalWrite(this->pin, 1);
#endif
      this->state = State::on;
      return 1;
    }
    else
      return 0;
  }

  bool Buzzer::off() {
    if (this->state == State::on) {
#ifdef ENABLE_GATEWAY
      digitalWrite(this->pin, 0);
#endif
      this->state = State::off;
      return 1;
    }
    else
      return 0;
  }

  Buzzer::State Buzzer::getState() {
    return this->state;
  }

  Buzzer::State Buzzer::flip() {
    if (this->state == State::on) {
      this->state = State::off;
#ifdef ENABLE_GATEWAY
      digitalWrite(this->pin, 0);
#endif
    }
    else {
      this->state = State::on;
#ifdef ENABLE_GATEWAY
      digitalWrite(this->pin, 1);
#endif
    }
    return this->state;
  }
}

