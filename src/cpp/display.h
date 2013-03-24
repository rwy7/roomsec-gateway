
/**
 * @file display.h Define the class display, and a library of
 * functions for putting messages on a display.
 */

#ifndef _ROOMSEC_DISPLAY_H_
#define _ROOMSEC_DISPLAY_H_

//TODO: what is this header file here?
//#include "display-colors.h"

#include <utility>

namespace roomsec {

  /**
   * Represents the screen as a 2D array of characters.  Addition
   * implementation and configurations shall be implemented by
   * concrete derived classes.
   */
  class Display {
  public:

    /**
     * Display backlight colors.  These values are accepted by the
     * display, when setting the display backlight value.
     */
    enum class Color : unsigned char {
      green = 0, yellow = 1, red = 3};

    //TODO: what is screen?
    // virtual Display(const Screen& that) = delete;

    virtual ~Display();

    /**
     * Set the backlight color of the display.  This call should
     * update the screen immediately.
     */
    virtual void setColor(Color c);

    /**
     * @brief set the cursor to [row, col].
     */
    virtual void setDisplay(int row, int col);

    /**
     * @brief Put the character onto the lcd at the current position
     *
     * @param character
     */
    virtual void putChar(char character);

    /**
     * @brief Clear the lcd screen.
     */
    virtual void clear();

    /**
     * @brief Move the cursor to [0,0]
     */
    virtual void home();

    /**
     * Returns the size (rows, cols)
     */
    virtual std::pair<int, int> size();

    /**
     * Returns the height of the display in character rows.
     */
    virtual int rows();

    /**
     * Returns the width of screen in character columns.
     */
    virtual int cols();

  };
}

#endif /* _ROOMSEC_DISPLAY_H_ */
