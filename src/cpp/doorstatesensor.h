#ifndef _DOORSTATESENSOR_H_
#define _DOORSTATESENSOR_H_

#include <mutex>

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
      DoorStateSensor(DoorStateSensor const& other) = delete;
      DoorStateSensor(DoorStateSensor && other) = delete;
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

    private:
      std::mutex mutex;
      State state;
      int pin;
      void update();
  };

  using DoorState = DoorStateSensor::State;
}

#endif /* _DOORSTATESENSOR_H */

