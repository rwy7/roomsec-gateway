#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "mockblocksensor.h"
#include "blockanalyzer.h"
#include <cstdio>


namespace roomsec {
  using namespace ::testing;

	int testIndex1 = -1, testIndex2 = -1;
	std::vector<int> testValues1 = {0,0,2,5,10,30,10,5,2,0,0,2,5,10,30,10,5,2,0,0,2,5,10,30,10,5,2,0};
	std::vector<int> testValues2 = {0,2,5,10,30,10,5,2,0,0,0,0,2,5,10,30,10,5,2,0};
	int testEndValue = 0;
	int mockSensorValue1()
	{
		testIndex1++;
		if(testIndex1 < testValues1.size())
			return testValues1[testIndex1];
		else
			return testEndValue;
	}
	int mockSensorValue2()
	{
		testIndex2++;
		if(testIndex2 < testValues2.size())
			return testValues2[testIndex2];
		else
			return testEndValue;
	}

  TEST(BlockAnalyzerTest, general) {
    MockBlockSensor blockSensor1, blockSensor2;

	std::vector<BlockSensor*> sensors = {&blockSensor1,&blockSensor2};
	BlockAnalyzer blockAna(sensors, true);

    EXPECT_CALL(blockSensor1, getSensorValue())
      .Times(AtLeast(1))
      .WillRepeatedly(Invoke(mockSensorValue1));

	EXPECT_CALL(blockSensor2, getSensorValue())
      .Times(AtLeast(1))
      .WillRepeatedly(Invoke(mockSensorValue2));

	blockAna.beginMonitoringSession();
	for(int i = 0; i < 30; i++)
		blockAna.update();
	blockAna.endMonitoringSession();

	PassageTriple result = blockAna.getResults();
	fprintf(stderr, "## Test Result : %i, %i, %i\n", result.ingoing, result.outgoing, result.unknown);
  }
}
