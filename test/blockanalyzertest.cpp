#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "mockblocksensor.h"
#include "blockanalyzer.h"
#include <cstdio>


namespace roomsec {
  using namespace ::testing;

	int testIndex = -1;
	std::vector<int> testValues = {0,0,2,5,10,30,30,10,5,2,0,0,2,5,10,30,30,10,5,2,0,0};
	int testEndValue = 0;
	int mockSensorValue()
	{
		testIndex++;
		if(testIndex < testValues.size())
			return testValues[testIndex];
		else
			return testEndValue;
	}

  TEST(BlockAnalyzerTest, general) {
    MockBlockSensor blockSensor1;

	std::vector<BlockSensor*> sensors = {&blockSensor1};
	BlockAnalyzer blockAna(sensors, true);

    EXPECT_CALL(blockSensor1, getSensorValue())
      .Times(AtLeast(1))
      .WillRepeatedly(Return(mockSensorValue()));

	blockAna.beginMonitoringSession();
	for(int i = 0; i < 10; i++)
		blockAna.update();
	blockAna.endMonitoringSession();

	PassageTriple result = blockAna.getResults();
	fprintf(stderr, "## Test Result : %i\n", result.unknown);
  }
}
