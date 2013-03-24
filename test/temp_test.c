#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

// MCP23S17 Registers
//
#define IOCON           0x0A

#define IODIRA          0x00
#define IPOLA           0x02
#define GPINTENA        0x04
#define DEFVALA         0x06
#define INTCONA         0x08
#define GPPUA           0x0C
#define INTFA           0x0E
#define INTCAPA         0x10
#define GPIOA           0x12
#define OLATA           0x14

#define IODIRB          0x01
#define IPOLB           0x03
#define GPINTENB        0x05
#define DEFVALB         0x07
#define INTCONB         0x09
#define GPPUB           0x0D
#define INTFB           0x0F
#define INTCAPB         0x11
#define GPIOB           0x13
#define OLATB           0x15

// Bits in the IOCON register

#define IOCON_BANK_MODE 0x80
#define IOCON_MIRROR    0x40
#define IOCON_SEQOP     0x20
#define IOCON_DISSLW    0x10
#define IOCON_HAEN      0x08
#define IOCON_ODR       0x04
#define IOCON_INTPOL    0x02
#define IOCON_UNUSED    0x01

// Default initialisation mode

#define IOCON_INIT      (IOCON_SEQOP)

/*  LCD Controls */
#define	LCD_CLEAR	0x01
#define	LCD_HOME	0x02
#define	LCD_ENTRY	0x04
#define	LCD_ON_OFF	0x08
#define	LCD_CDSHIFT	0x10
#define	LCD_FUNC	0x20
#define	LCD_CGRAM	0x40
#define	LCD_DGRAM	0x80

#define	LCD_ENTRY_SH	0x01
#define	LCD_ENTRY_ID	0x02

#define	LCD_ON_OFF_B	0x01
#define	LCD_ON_OFF_C	0x02
#define	LCD_ON_OFF_D	0x04

#define	LCD_FUNC_F	0x04
#define	LCD_FUNC_N	0x08
#define	LCD_FUNC_DL	0x10

#define	LCD_CDSHIFT_RL	0x04

/*  MSCP23017 -> LCD pin mapping */
#define LCD_DB4  0x01
#define LCD_DB5  0x02
#define LCD_DB6  0x04
#define LCD_DB7  0x08
#define LCD_E    0x10
#define LCD_RS   0x20

void strobe (int dev) {
  wiringPiI2CWriteReg8(dev, GPIOA, LCD_E);
  delay(50);
/*
for (;;) {
wiringPiI2CWriteReg8 (dev, GPIOA, LCD_E);
delay(500);
wiringPiI2CWriteReg8(dev, GPIOA, 0);
delay(500);
}
*/
  wiringPiI2CWriteReg8(dev, GPIOA, 0);
  delay(50);
}

static void put4Command (int dev, uint8_t command)
{
  uint8_t i ;
  command = command;
wiringPiI2CWriteReg8 (dev, GPIOA, command); 
  strobe (dev) ;
}

static void sendDataCmd (int dev, uint8_t data)
{
  uint8_t d4 ;
  d4 = (data >> 4) & 0x0F;

  wiringPiI2CWriteReg8 (dev, GPIOA, d4); 

  strobe (dev) ;

  d4 = data & 0x0F ;
 wiringPiI2CWriteReg8 (dev, GPIOA, d4); 

  strobe (dev) ;
}

static void sendCharCmd (int dev, uint8_t data) {
  uint8_t d4 ;
  d4 = (data >> 4) & 0x0F | LCD_RS;

  wiringPiI2CWriteReg8 (dev, GPIOA, d4); 

  wiringPiI2CWriteReg8 (dev, GPIOA, d4 | LCD_E); 
  //strobe (dev) ;

  d4 = data & 0x0F | LCD_RS;
 wiringPiI2CWriteReg8 (dev, GPIOA, d4); 

  wiringPiI2CWriteReg8 (dev, GPIOA, d4 | LCD_E); 
  //strobe (dev) ;
}

int main (int argc, char *argv [])
{
  int dev;

  if (wiringPiSetup () == -1)
    { fprintf (stderr, "Unable to initialise wiringPi: %s\n", strerror (errno)) ; return 1 ; }

  if ((dev = wiringPiI2CSetup (0x20)) == -1)
    { fprintf (stderr, "Unable to initialise I2C: %s\n", strerror (errno)) ; return 1 ; }

  /*  Initialize the MCP23017 */
  wiringPiI2CWriteReg8 (dev, IOCON, IOCON_INIT) ;
  wiringPiI2CWriteReg8 (dev, IODIRA, 0x00) ;		// Port A -> Outputs
  wiringPiI2CWriteReg8 (dev, IODIRB, 0x00) ;		// Port B -> Outputs

  /*  Initialize the LCD */
  int func;
  func = LCD_FUNC | LCD_FUNC_DL;

  func = LCD_FUNC | LCD_FUNC_DL ;			// Set 8-bit mode 3 times
  put4Command (dev, func >> 4) ; delay (35) ;
  put4Command (dev, func >> 4) ; delay (35) ;
  put4Command (dev, func >> 4) ; delay (35) ;
  func = LCD_FUNC ;					// 4th set: 4-bit mode
  put4Command (dev, func >> 4) ; delay (35) ;

  func |= LCD_FUNC_N ;
  sendDataCmd (dev, func) ; delay (35) ;
 
sendDataCmd (dev, LCD_HOME);
sendDataCmd (dev, LCD_CLEAR);
sendCharCmd (dev, 'A'); 
sendCharCmd (dev,'b');
  return 0 ;
}

