#ifndef _LCDDISPLAY_H_
#define _LCDDISPLAY_H_

#include "boost/shared_ptr.hpp"
#include "ioexpander.h"
#include "display.h"

namespace roomsec {


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

    protected:
    void strobe();
    void sendDataCmd(uint8_t data);
    void putCommand(uint8_t data);
    void put4Command(uint8_t data);

    boost::shared_ptr<IOExpander> expander;
  };
}

#endif
