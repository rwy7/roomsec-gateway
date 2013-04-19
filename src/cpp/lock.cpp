#include "config.h"
#include <boost/shared_ptr.hpp>
#include "ioexpander.h"
#include "lock.h"

namespace roomsec {
  Lock::Lock(boost::shared_ptr<IOExpander> device, IOExpander::GPIO bank, uint8_t pins)
    : device(device), bank(bank), pins(pins), state(LockState::locked)
  {
    /*  set the pins to write mode */
    this->device->setRW(this->bank, (this->device->getRW(this->bank) & ~this->pins));
    this->device->makeLow(this->bank, this->pins);
    return;
  }

  LockState Lock::getState() {
    return this->state;
  }

  bool Lock::setState(LockState state) {
    /* Check if the desired state is not the current state.  Set the
     * current state if it needs to be set. Return True if something
     * changed.
     *
     * The lock is locked when the pin is low, and open when the pin
     * is high.
     */
    bool result = (this->state != state);
    if (result) {
      this->state = state;
      switch (state) {
      case LockState::locked:
	this->device->makeLow(this->bank, this->pins);
	break;
      case LockState::unlocked:
	this->device->makeHigh(this->bank, this->pins);
	break;
      }
    }
    return result;
  }
}
