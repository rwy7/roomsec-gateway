
/**
 * @file display.h Define the class display, and a library of
 * functions for putting messages on a display.
 */

#ifndef _ROOMSEC_DISPLAY_H_
#define _ROOMSEC_DISPLAY_H_

#include "display-colors.h"

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

    virtual Display(const Screen& that) = delete;

    virtual ~Display();

    /**
     * Ste the backlight color of the display.  This call should
     * update the screen immediately.
     */
    virtual setColor(Color c);

    /**
     * set the character located at [row, col].  This character is
     * buffered, and should not be displayed untill the screen is
     * flipped.
     */
    virtual setDisplay(int row, int col, char c);

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

    /**
     * Flip the screen buffer, dumping all characters to screen.
     */
    virtual refresh();
  };
}

#endif /* _ROOMSEC_DISPLAY_H_ */
