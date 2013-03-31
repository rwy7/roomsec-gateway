#ifndef MCP3008BLOCKSENSOR
#define MCP3008BLOCKSENSOR

#include "BlockSensor.h"
#include <wiringPi/wiringPiSPI.h>

#define ADC_CHANNEL 0

#define ADC_HIGH_VOLTAGE 3.3
#define ADC_LOW_VOLTAGE 0
#define ADC_HIGH_DATA 1023
#define MONITORED_CLOSE_VOLTAGE 3.0
#define MONITORED_FAR_VOLTAGE 0.4
#define INPUT_CLOSE_VALUE 100
#define INPUT_FAR_VALUE 0

namespace roomsec{

class MCP3008BlockSensor : BlockSensor{
protected:
	unsigned int MCPPin; //0-7
	int iValue, mValue;
	bool DEBUG;

public:
	MCP3008BlockSensor(unsigned int pin, bool debug = false):MCPPin(pin),mValue(0),iValue(0),DEBUG(debug){}

	int getSensorValue();
}

}

#endif
