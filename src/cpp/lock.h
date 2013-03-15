#ifndef _ROOMSEC_LOCK_H_
#define _ROOMSEC_LOCK_H_

namespace roomsec {

  /**
   * A representation of a physical lock device.  In the gateway
   * system, the lock is actually implemented as an electronically
   * controlled strike plate.  This class provides an abstract
   * interface to the lock object.
   */
  class Lock {
  public:
    
    /**
     * The state of a door lock.  The lock is a physical device attached
     * to the gateway.  The lock may either be locked, or unlocked.
     */
    enum class LockState {
      locked, unlocked
    };

    /**
     * Determine the locks' state.
     */
    LockState getState();

    /**
     * Set the state of the lock.
     */
    setState(LockState state);
  };
}

#endif /* _ROOMSEC_LOCK_H_ */
