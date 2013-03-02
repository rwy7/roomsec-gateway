#ifndef BLOCKSENSOR
#define BLOCKSENSOR

class BlockSensor{
	protected:
	static const iMin = 0, iMax = 100;
	float mSensorValue;
	int iSensorValue
	public:
	virtual int getSensorValue();//TODO implement this sh*t
};	

#endif
