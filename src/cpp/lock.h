#ifndef _ROOMSEC_LOCK_H_
#define _ROOMSEC_LOCK_H_

#include <boost/shared_ptr.hpp>
#include "ioexpander.h"

namespace roomsec {

  /**
   * The state of a door lock.  The lock is a physical device attached
   * to the gateway.  The lock may either be locked, or unlocked.
   */
  enum class LockState {
    locked, unlocked
  };

  /**
   * A representation of a physical lock device.  In the gateway
   * system, the lock is actually implemented as an electronically
   * controlled strike plate.  This class provides an abstract
   * interface to the lock object.
   */
  class Lock {
  public:

    /**
     * @brief Create a lock device.
     * The device is expected to be sitting on an I2C ioexpander device.
     *
     * @param device The I2C ioexpander device.
     * @param bank The IOExpander GPIO bank in use.
     * @param pin The pin(s) on the bank.
     */
    Lock(boost::shared_ptr<IOExpander> device, IOExpander::GPIO bank, uint8_t pin);

    /**
     * Determine the locks' state.
     */
    LockState getState();

    /**
     * Set the state of the lock.
     *
     * @returns True if the lock state has changed
     */
    bool setState(LockState state);

  private:
    boost::shared_ptr<IOExpander> device;
    IOExpander::GPIO bank;
    uint8_t pins;
    LockState state;
  };
}

#endif /* _ROOMSEC_LOCK_H_ */
