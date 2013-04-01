#ifndef BLOCKSENSOR
#define BLOCKSENSOR

namespace roomsec{

	/**
	 * @brief This class provides an interface for a photoelectric distance sensor
	 *
	 */
class BlockSensor{
	protected:
	//static const int iLow = 0, iHigh = 100;
	//float mSensorValue;
	//int iSensorValue;

	public:
	virtual ~BlockSensor(){}

	/**
	 * @brief returns the voltage signal from the sensor mapped to an integer range from iLow..iHigh
	 *
	 * The lowest value corresponds to the signal for the maximum detection range on the sensor.
	 * The highest value corresponds to the signal for the minimum detection range on the sensor.
	 */
	virtual int getSensorValue()=0;
};

}	

#endif
