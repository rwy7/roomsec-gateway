#ifndef _ROOMSEC_MOCKBLOCKSENSOR_H_
#define _ROOMSEC_MOCKBLOCKSENSOR_H_

#include "gmock/gmock.h"
#include "blocksensor.h"

namespace roomsec{
	class MockBlockSensor : public BlockSensor
	{
		public:
		MOCK_METHOD0(getSensorValue, float());
	};
}

#endif /* _ROOMSEC_MOCKBLOCKSENSOR_H_ */
