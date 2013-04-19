#include "config.h"
#include <boost/shared_ptr.hpp>
#include <wiringPi/wiringPi.h>
#include "ioexpander.h"
#include "display.h"
#include "lcddisplay.h"

namespace roomsec {

  /** LCD Commands. These are signals which are sent to the LCD's 8 ports.  In
   * 4 bit mode, they are sent in two rounds of four.
   *
   * This device is Hitachi HD44780U compatable, so please see
   * http://adafruit.com/datasheets/HD44780.pdf for detalied controls.
   *
   * For a pin reading, please see:
   * http://adafruit.com/datasheets/rgblcddimensions.gif */

  enum LCD_COMMANDS {
    /*  LCD Controls */
    LCD_CLEAR    = 0x01,
    LCD_HOME     = 0x02,
    LCD_ENTRY    = 0x04,
    LCD_ON_OFF   = 0x08,
    LCD_CDSHIFT  = 0x10,
    LCD_FUNC     = 0x20,
    LCD_CGRAM    = 0x40,
    LCD_DGRAM    = 0x80,

    LCD_ENTRY_SH = 0x01,
    LCD_ENTRY_ID = 0x02,

    LCD_ON_OFF_B = 0x01,
    LCD_ON_OFF_C = 0x02,
    LCD_ON_OFF_D = 0x04,

    LCD_FUNC_F   = 0x04,
    LCD_FUNC_N   = 0x08,
    LCD_FUNC_DL  = 0x10,

    LCD_CDSHIFT_RL = 0x04,

    /*  MSCP23017 -> LCD pin mapping */
    LCD_DB4 = 0x01,
    LCD_DB5 = 0x02,
    LCD_DB6 = 0x04,
    LCD_DB7 = 0x08,
    LCD_E   = 0x10,
    LCD_RS  = 0x20
  };

  void LCDDisplay::strobe() {
    /*  Strobe will send the next command, by cycling the enable pin.  The data
     *  is latched on the falling edge, according to the docs. */
    this->expander->makeHigh(this->expander->GPIOA, LCD_E); delayMicroseconds
      (100);
    this->expander->makeLow (this->expander->GPIOA, LCD_E);  delayMicroseconds
      (100);
  }

  void LCDDisplay::sendDataCmd(uint8_t data) {
    uint8_t d4;
    /*  We must send the 8 bit command over two 4 bit commands. d4 holds the
     *  4bit version. */
    d4 = (data >> 4) & 0x0F;
    this->expander->makeLow (this->expander->GPIOA, 0x0F);
    this->expander->makeHigh(this->expander->GPIOA, d4);
    this->strobe();
    d4 = data & 0x0F;

    this->expander->makeLow (this->expander->GPIOA, 0x0F);
    this->expander->makeHigh(this->expander->GPIOA, d4);
    this->strobe();
  }

  void LCDDisplay::putCommand(uint8_t data) {
    this->expander->makeLow(this->expander->GPIOA, LCD_RS);
    this->sendDataCmd(data);
  }

  void LCDDisplay::put4Command(uint8_t data) {
    this->expander->makeLow(this->expander->GPIOA, LCD_RS | 0x0F);
    this->expander->makeHigh(this->expander->GPIOA, data & 0x0F);
    this->strobe();
  }

  LCDDisplay::LCDDisplay (boost::shared_ptr<IOExpander> exp)
    : expander(exp) {

    }

  LCDDisplay::~LCDDisplay () {
    this->setBacklightColor(Color::none);
    this->clear();
    this->home();
    return;
  }

  void LCDDisplay::initialize() {
    /*  Make sure that the expander is set up to write mode only */
    this->expander->setRW(this->expander->GPIOA, 0x00);
    this->expander->makeLow(this->expander->GPIOA, 0x3F);

    /*  The device will initialize in 4 pin mode. */
    uint8_t func;
    func = LCD_FUNC | LCD_FUNC_DL;
    put4Command (func >> 4); delay(35);
    put4Command (func >> 4); delay(35);
    put4Command (func >> 4); delay(35);
    func = LCD_FUNC ;
    put4Command (func >> 4); delay(35);

    func |= LCD_FUNC_N;
    putCommand(func); delay(35);

    putCommand(LCD_ON_OFF | LCD_ON_OFF_D); delay(2);
    putCommand(LCD_ENTRY | LCD_ENTRY_ID); delay(2);
    // putCommand(LCD_CDSHIFT  | LCD_CDSHIFT_RL); delay(2);
    //putCommand(LCD_CLEAR); delay(5);

    /*  Test out the display */
    sendDataCmd(LCD_HOME); delay(5);
    sendDataCmd(LCD_CLEAR); delay (5);
    this->expander->makeHigh(this->expander->GPIOA, LCD_RS);
  }

  void LCDDisplay::setBacklightPins(IOExpander::GPIO gpio, uint8_t red, uint8_t
      green, uint8_t blue) {
    this->colorGPIO = gpio;
    this->expander->setRW(this->colorGPIO, ~(this->color[red] |
          this->color[blue] | this->color[green]));

    this->color[this->red] = red;
    this->color[this->green] = green;
    this->color[this->blue] = blue;
    this->setBacklightColor (Color::none);
    return;
  }

  void LCDDisplay::setBacklightColor(Color color) {
    this->expander->makeLow(this->colorGPIO, this->color[red] |
        this->color[blue] | this->color[green]);
    if (color != -1)
      this->expander->makeHigh(this->colorGPIO, this->color[color]);
    return;
  }

  void LCDDisplay::setDisplay(int row, int col) {
    // These are the offset addresses of the 4 possible rows of an LCD display
    // Row addresses are not contiguous
    const static uint8_t rowOff [4] = {
      0x00, 0x40, 0x14, 0x54
    };
    assert(row <= 4 && row >= 0);
    assert(col <= 16 && col >= 0);
    this->putCommand(LCD_DGRAM | rowOff[row] | col);
  }

  void LCDDisplay::putChar(char character){
    this->expander->makeHigh(this->expander->GPIOA, LCD_RS);
    sendDataCmd(character);
    return;
  }

  void LCDDisplay::putStr(std::string const& stuff) {
    std::string::const_iterator it;
    for (it = stuff.begin(); it < stuff.end(); ++it) {
      this->putChar(*it);
    }
  }

  void LCDDisplay::clear() {
    putCommand(LCD_CLEAR);
    return;
  }

  void LCDDisplay::home() {
    putCommand(LCD_HOME);
    return;
  }

  std::pair<int, int> LCDDisplay::size(){ return std::make_pair(0, 0);}
  int LCDDisplay::rows() {return 0;}
  int LCDDisplay::cols() {return 0;}
}

