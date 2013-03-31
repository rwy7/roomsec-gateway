#ifndef _DOORSTATESENSOR_H_
#define _DOORSTATESENSOR_H_

namespace roomsec {

  /**
   * @brief Senses if a door is opened or closed.
   */
  class DoorStateSensor {

    public:

      /**
       * @brief Setup a door sensor.
       * This will try to set the pin to READ mode.  If not running with
       * the correct priviledges, make sure thhat the pin is already in
       * this mode.
       * @param pin The GPIO pin which the switch is connected to.
       */
      DoorStateSensor(int pin);

      ~DoorStateSensor();

      /**
       * @brief Whether the door is open or closed.
       */
      enum class State {open, closed};

      /**
       * @brief Get the current door state.
       *
       * This will poll the sensor, ensuring up-to-date information.  No
       * results are memoized.
       *
       * @return The current door state.
       */
      State getDoorState();

      /**
       * @brief Sleep until the state changes.
       * This will put the thread to sleep until there is a hardware interrupt
       * indicating a change in state.
       * @return The new (current) state.
       */
      int waitForChange(void (*callback)(void));
    private:
      State state;
      int pin;
      void update();
  };
}

#endif /* _DOORSTATESENSOR_H */

