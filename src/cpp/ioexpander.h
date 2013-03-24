#ifndef _IOEXPANDER_H_
#define _IOEXPANDER_H_

#include <cstdint>

namespace roomsec {

  /**
   * @brief 16 bit IO Expander.
   * Communication happens over I2C.  Made specifically for the MCP23017
   * hardware device. The channels are separated into two banks: A and B. Each
   * IO pin is accessed as a bit in a uint8_t. The device must be initialized
   * before it can be used.  All pins are set to read mode initially.
   *
   * For more information on the MCP23017, please see
   * http://ww1.microchip.com/downloads/en/devicedoc/21952b.pdf
   */
  class IOExpander {


    public:

      /**
       * @brief Create a device handler
       *
       * @param dev The I2C device number.
       */
    IOExpander (int dev);

    ~IOExpander ();

    /**
     * @brief The GPIO bank of the device.
     * There are two banks on each device, GPIOA and GPIOB. On each bank there
     * are 8 pins.  Each pin can be read to or written to.
     */
    enum GPIO {A = 0x12, B = 0x13};

    /**
     * @brief Initial the device.
     * This must be called before the device can be used.  It will set all pins
     * to read mode.
     */
    void initialize();

    /**
     * @brief Set each pin to READ or WRITE mode.
     * If a pin is 0, it is set to read mode.
     * If a pin is 1, it is set to write mode.
     *
     * @param bank The GPIO bank.
     * @param pins The pin R/W setting.
     */
    void setRW (GPIO bank, uint8_t pins);

    /**
     * @brief Get whether pins are READ or WRITE mode.
     * See setIO for more iformation.
     *
     * @param bank The GPIO bank.
     *
     * @return The pin R/W setting.
     */
    uint8_t getRW (GPIO bank);

    /**
     * @brief Make the specified pins HIGH.
     * This will leave unspecified pins at the same value.
     *
     * @param bank The GPIO bank.
     * @param pins The pins to be set to HIGH.
     */
    void makeHigh (GPIO bank, uint8_t pins);

    /**
     * @brief Make the specified pins LOW.
     * This will leave unspecified pins at the same value.
     *
     * @param bank The GPIO bank.
     * @param pins The pins to be set to LOW.
     */
    void makeLow (GPIO bank, uint8_t pins);
    
    /**
     * @brief Write to all pins on a GPIO bank
     * Values written to read only pins will be ignored.
     *
     * @param bank The GPIO bank
     * @param value The value for each pin.
     */
    void send (GPIO bank, uint8_t value);

    /**
     * @brief Read all pins from a GPIO bank
     * This will give the value of all pins.
     * @param bank The GPIO bank.
     *
     * @return The value of each pin.
     */
    uint8_t read(GPIO bank);
  };
}

#endif /*  _IOEXPANDER_H_ */

