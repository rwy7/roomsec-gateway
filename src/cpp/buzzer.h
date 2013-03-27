#ifndef BUZZER_H
#define BUZZER_H

namespace roomsec {
  class Buzzer {
    public:

      /**
       * The state of the door
       */
      enum class State : int {off = 0, on = 1};

      /**
       * @brief Set up a piezo electric buzzer.
       * This will set the pin to output mode.
       *
       * @param pin The PGIO pin in the raspberry pi.
       */
      Buzzer(int pin);

      ~Buzzer();
      /**
       * @brief Turn the buzzer on.
       * This function will not write to the pin if it does not think it has
       * to.
       * @return True if the buzzer was previously off.
       */
      bool on();

      /**
       * @brief Turn the buzzer off.
       * This function will not write to the pin if it does not think it has
       * to.
       * @return  True if the buzzer was previously on.
       */
      bool off();

      /**
       * @brief Check if the buzzer is on.
       *
       * @return The state of the buzzer
       */
      Buzzer::State getState();

      /**
       * @brief Flip the buzzer state.
       * If to is on, it turns off, and vice versa.
       *
       * @return The new state of the buzzer.
       */
      Buzzer::State flip();

    private:
      /*  If the buzzer is currently on or not */
      Buzzer::State state;

      /*  The pin which the buzzer operates on */
      int pin;

  };
}

#endif
