#include <cstdio>
#include <wiringPi/wiringPiSPI.h>
#include "blocksensor.h"
#include "mcp3008blocksensor.h"
#include <log4cxx/logger.h>


#define ADC_CHANNEL 0
#define ADC_HIGH_VOLTAGE 3.3
#define ADC_LOW_VOLTAGE 0
#define ADC_HIGH_DATA 1023
#define MONITORED_CLOSE_VOLTAGE 3.0
#define MONITORED_FAR_VOLTAGE 0.4
#define INPUT_CLOSE_VALUE 100
#define INPUT_FAR_VALUE 0

namespace roomsec {
static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("roomsec.mcp3008blocksensor");

  MCP3008BlockSensor::MCP3008BlockSensor(unsigned int pin, bool debug)
  {
	MCPPin = pin;
	iValue = 0;
	mValue = 0;
	DEBUG = debug;
	LOG4CXX_DEBUG(logger, "MCP3008BlockSensor - construction complete");
  }


  MCP3008BlockSensor::~MCP3008BlockSensor() {
  }
      

  int MCP3008BlockSensor::getSensorValue() {
	LOG4CXX_DEBUG(logger, "MCP3008BlockSensor - getSensorValue()");
    int mVal, iVal;
		
    //read from sensor value to mVal using wiringPi SPI library
		
    unsigned char data[] = {1, (8 + MCPPin) << 4, 0};

    wiringPiSPIDataRW(ADC_CHANNEL, data, 3);
		
    mVal = ((data[1]&3) << 8) + data[2];		

    if(mVal != mValue) {	
      //map mVal from iLow-iHigh to iVal
      iVal = (mVal * ADC_HIGH_VOLTAGE / ADC_HIGH_DATA);

      iVal = (iVal - MONITORED_FAR_VOLTAGE) * 
	(INPUT_CLOSE_VALUE-INPUT_FAR_VALUE) / 
	(MONITORED_CLOSE_VOLTAGE-MONITORED_FAR_VOLTAGE);

      int low = (INPUT_FAR_VALUE < INPUT_CLOSE_VALUE) ? INPUT_FAR_VALUE : INPUT_CLOSE_VALUE;
      int high = (INPUT_FAR_VALUE > INPUT_CLOSE_VALUE) ? INPUT_FAR_VALUE : INPUT_CLOSE_VALUE;
      if (iVal < low)	
	iVal = low;
      if (iVal > high)
	iVal = high;

      mValue = mVal;
      iValue = iVal;
    }
		
    if (DEBUG)
      printf("#MCP3008BlockSensor::getSensorValue() - iValue = %i; mValue = %i;\n", iValue, mValue);

    return iValue;
  }
}
