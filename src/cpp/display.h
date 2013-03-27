
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

    enum Color {red = 0, green = 1, blue = 2, none = -1};

    virtual ~Display();

    /**
     * Set the backlight color of the display.  This call should
     * update the screen immediately.
     */
    virtual void setBacklightColor(Color color) = 0; 

    /**
     * @brief set the cursor to [row, col].
     */
    virtual void setDisplay(int row, int col) = 0;

    /**
     * @brief Put the character onto the lcd at the current position
     *
     * @param character
     */
    virtual void putChar(char character) = 0;

    /**
     * @brief put a string onto the lcd, from the current position.
     */
    virtual void putStr(std::string const& str) = 0;
    
    /**
     * @brief Clear the lcd screen.
     */
    virtual void clear() = 0;

    /**
     * @brief Move the cursor to [0,0]
     */
    virtual void home() = 0;

    /**
     * Returns the size (rows, cols)
     */
    virtual std::pair<int, int> size() = 0;

    /**
     * Returns the height of the display in character rows.
     */
    virtual int rows() = 0;

    /**
     * Returns the width of screen in character columns.
     */
    virtual int cols() = 0;

  };

  inline Display::~Display() {return;}

}

#endif /* _ROOMSEC_DISPLAY_H_ */
