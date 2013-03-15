#ifndef _DOORSTATESENSOR_H_
#define _DOORSTATESENSOR_H_

namespace roomsec {

  /**
   * @brief Whether the door is open or closed.
   */
  enum DoorState {open, closed};


  /**
   * @brief Senses if a door is opened or closed.
   */
  class DoorStateSensor {

    public:

      /**
       * @brief Get the current door state.
       *
       * This will poll the sensor, ensuring up-to-date information.  No
       * results are memoized.
       *
       * @return The current door state.
       */
      virtual DoorState getDoorState();

    private:
      DISALLOW_COPY_AND_ASSIGN(DoorStateSensor);

  };
}

#endif /* _DOORSTATESENSOR */

