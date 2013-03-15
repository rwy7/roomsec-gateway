#ifndef BLOCKSENSOR
#define BLOCKSENSOR

namespace roomsec{

class BlockSensor{
	protected:
	static const iLow = 0, iHigh = 100;
	float mSensorValue;
	int iSensorValue
	public:
	
	/**
	 * @brief returns the voltage signal from the sensor mapped to an integer range from iLow..iHigh
	 *
	 * iLow corresponds to the signal for the maximum detection range on the sensor.
	 * iHigh corresponds to the signal for the minimum detection range on the sensor.
	 */
	virtual int getSensorValue();
};

}	

#endif
