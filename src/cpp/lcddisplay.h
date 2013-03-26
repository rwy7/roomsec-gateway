/* -*- Mode: c++ -*- */
#ifndef _LCDDISPLAY_H_
#define _LCDDISPLAY_H_

#include "boost/shared_ptr.hpp"
#include "display.h"

namespace roomsec {

  class IOExpander;

  /**
   * @brief An LCD Screen attached to a I2C IOExpander.
   * The LCD requires a specific setup to work - there is not many options
   * here.
   */
  class LCDDisplay : public Display {
  public:
    LCDDisplay(boost::shared_ptr<IOExpander> expander);
    ~LCDDisplay();
    void initialize();

    /**
     * Set the backlight color of the display.  This call should
     * update the screen immediately.
     */
    void setColor(Color c);

    /**
     * @brief set the cursor to [row, col].
     */
    void setDisplay(int row, int col);

    /**
     * @brief Put the character onto the lcd at the current position
     *
     * @param character
     */
    void putChar(char character);

    void putStr(std::string stuff);

    /**
     * @brief Clear the lcd screen.
     */
    void clear();

    /**
     * @brief Move the cursor to [0,0]
     */
    void home();

    /**
     * Returns the size (rows, cols)
     */
    std::pair<int, int> size();

    /**
     * Returns the height of the display in character rows.
     */
    int rows();

    /**
     * Returns the width of screen in character columns.
     */
    int cols();

  protected:
    void strobe();
    void sendDataCmd(uint8_t data);
    void putCommand(uint8_t data);
    void put4Command(uint8_t data);

    boost::shared_ptr<IOExpander> expander;
  };
}

#endif
