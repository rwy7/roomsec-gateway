#include "mcp3008blocksensor.h"

int MCP3008BlockSensor::getSensorValue()
	{
		int mVal, iVal;
		
		//read from sensor value to mVal using wiringPi SPI library
		
		unsigned char *data;

		mVal = wiringPiSPIDataRW(ADC_CHANNEL, data, 0);
				

		if(mVal != mValue)
		{	
			//map mVal from iLow-iHigh to iVal
			iVal = (mVal * ADC_HIGH_VOLTAGE / ADCHIGH_DATA);
			iVal = (iVal-MONITORED_FAR_VOLTAGE) * (INPUT_FAR_VALUE-INPUT_CLOSE_VALUE) / (MONITORED_CLOSE_VOLTAGE-MONITORED_FAR_VOLTAGE);
			int low = (INPUT_FAR_VALUE < INPUT_CLOSE_VALUE) ? INPUT_FAR_VALUE : INPUT_CLOSE_VALUE;
			int high = (INPUT_FAR_VALUE > INPUT_CLOSE_VALUE) ? INPUT_FAR_VALUE : INPUT_CLOSE_VALUE;
			if (iVal < low)	
				iVal = low;
			if (iVal > high)
				iVal = high;

			mValue = mVal;
			iValue = iVal
		}
		
		if (DEBUG)
			printf("#MCP3008BlockSensor::getSensorValue() - iValue = %i; mValue = %i;\n", iValue, mValue);

		return iValue;
	}