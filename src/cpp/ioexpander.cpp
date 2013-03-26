#include "config.h"

#include <iostream>
#include <cassert>

#ifdef ENABLE_GATEWAY
#include <wiringPi/wiringPi.h>
#include <wiringPi/wiringPiI2C.h>
#endif /* ENABLE_GATEWAY */

#include "ioexpander.h"

namespace roomsec {
  /* MCP23017 Registers
   * See the manual for more information about what these mean 
   */
  enum {
    IOCON          = 0x0A,

    IODIRA         = 0x00,
    IPOLA          = 0x02,
    GPINTENA       = 0x04,
    DEFVALA        = 0x06,
    INTCONA        = 0x08,
    GPPUA          = 0x0C,
    INTFA          = 0x0E,
    INTCAPA        = 0x10,
    //GPIOA        = 0x12, (in the header file)
    OLATA          = 0x14,

    IODIRB        =  0x01,
    IPOLB         =  0x03,
    GPINTENB      =  0x05,
    DEFVALB       =  0x07,
    INTCONB       =  0x09,
    GPPUB         =  0x0D,
    INTFB         =  0x0F,
    INTCAPB       =  0x11,
    //GPIOB       =  0x13, (in the header file)
    OLATB         =  0x15,

    // Bits in the IOCON register
    IOCON_BANK_MODE = 0x80,
    IOCON_MIRROR    = 0x40,
    IOCON_SEQOP     = 0x20,
    IOCON_DISSLW    = 0x10,
    IOCON_HAEN      = 0x08,
    IOCON_ODR       = 0x04,
    IOCON_INTPOL    = 0x02,
    IOCON_UNUSED    = 0x01,

    /*  Default Settings */
    IOCON_INIT      = (IOCON_SEQOP)
  };

  IOExpander::IOExpander () : gpioa(0), gpiob(0), iodira(0), iodirb(0) {
  }

  IOExpander::~IOExpander () {

  }

  void IOExpander::initialize (int dev) {
#ifdef ENABLE_GATEWAY
    if ((dev = wiringPiI2CSetup (dev)) == -1) {
      std::cout << "Unable to create device";
      assert (dev > 0);
      //TODO: raise an exception
      //TODO: Log something
    }
#endif

    this->dev = dev;

#ifdef ENABLE_GATEWAY
    /*  Initialize the device */
    wiringPiI2CWriteReg8 (dev, IOCON, IOCON_INIT);
    wiringPiI2CWriteReg8 (dev, IODIRA, 0x00);
    wiringPiI2CWriteReg8 (dev, IODIRB, 0x00);
    wiringPiI2CWriteReg8 (dev, GPIOA, 0x00);
    wiringPiI2CWriteReg8 (dev, GPIOB, 0x00);
#endif

    this->gpioa = 0x00;
    this->gpiob = 0x00;
    this->iodira = 0x00;
    this->iodirb = 0x00;

    return;
  }

#ifdef ENABLE_GATEWAY
  void IOExpander::setRW (GPIO bank, uint8_t pins) {
    assert (bank == GPIOA || bank == GPIOB);
    if (bank == GPIOA) {

      wiringPiI2CWriteReg8 (dev, IODIRA, pins);
      iodira = pins;
    }
    else if (bank == GPIOB) {
      wiringPiI2CWriteReg8 (dev, IODIRB, pins);
      iodirb = pins;
    }
  }
#else
  void IOExpander::setRW (GPIO bank, uint8_t pins) {
    assert (bank == GPIOA || bank == GPIOB);
    if (bank == GPIOA) {
      iodira = pins;
    }
    else if (bank == GPIOB) {
      iodirb = pins;
    }
  }
#endif

  uint8_t IOExpander::getRW (GPIO bank) {
    assert (bank == GPIOA || bank == GPIOB);
    uint8_t rw = 0;
    if (bank == GPIOA) {
      rw = iodira;
    }
    else if (bank == GPIOB) {
      rw = iodirb;
    }
    return rw;
  }

#ifdef ENABLE_GATEWWAY
  void IOExpander::makeHigh (GPIO bank, uint8_t pins) {
    assert (bank == GPIOA || bank == GPIOB);
    uint8_t *gpio = NULL;
    if (bank == GPIOA) {
      gpio = &this->gpioa;
    }
    else if (bank == GPIOB) {
      gpio = &this->gpiob;
    }
    else
      assert (1);

    *gpio |= pins;

    wiringPiI2CWriteReg8 (this->dev, bank, *gpio);
    return;
  }
#else
  void IOExpander::makeHigh (GPIO bank, uint8_t pins) {
    assert (bank == GPIOA || bank == GPIOB);
    uint8_t *gpio = NULL;
    if (bank == GPIOA) {
      gpio = &this->gpioa;
    }
    else if (bank == GPIOB) {
      gpio = &this->gpiob;
    }
    else
      assert (1);

    *gpio |= pins;
    return;
  }

#endif

#ifdef ENABLE_GATEWAY
  void IOExpander::makeLow (GPIO bank, uint8_t pins) {
    assert (bank == GPIOA || bank == GPIOB);
    uint8_t *gpio = NULL;
    if (bank == GPIOA) {
      gpio = &this->gpioa;
    }
    else if (bank == GPIOB) {
      gpio = &this->gpiob;
    }
    else
      assert (1);

    *gpio &= ~pins;
    wiringPiI2CWriteReg8 (this->dev, bank, *gpio);
    return;
  }
#else
  void IOExpander::makeLow (GPIO bank, uint8_t pins) {
    assert (bank == GPIOA || bank == GPIOB);
    uint8_t *gpio = NULL;
    if (bank == GPIOA) {
      gpio = &this->gpioa;
    }
    else if (bank == GPIOB) {
      gpio = &this->gpiob;
    }
    else
      assert (1);

    *gpio &= ~pins;
    return;
  }
#endif

  void IOExpander::send (GPIO bank, uint8_t value) {

    assert (bank == GPIOA || bank == GPIOB);
#ifdef ENABLE_GATEWAY
    wiringPiI2CWriteReg8 (this->dev, bank, value);
#endif
    return;
  }

  uint8_t IOExpander::read(GPIO bank) {
    assert (bank == GPIOA || bank == GPIOB);
#ifdef ENABLE_GATEWAY
    return wiringPiI2CReadReg8 (dev, bank);
#else
    assert(0);
    return 0;
#endif
  }

} /*  namespace roomsec */

