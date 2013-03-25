#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "mockblocksensor.h"
#include "blockanalyzer.h"
#include <cstdio>


namespace roomsec {
  using namespace ::testing;

  TEST(BlockAnalyzerTest, general) {
    MockBlockSensor blockSensor1;

	std::vector<BlockSensor*> sensors = {&blockSensor1};
	BlockAnalyzer blockAna(sensors, true);

    EXPECT_CALL(blockSensor1, getSensorValue())
      .Times(AtLeast(1))
      .WillRepeatedly(Return(25));

	blockAna.beginMonitoringSession();
	for(int i = 0; i < 10; i++)
		blockAna.update();
	blockAna.endMonitoringSession();

	PassageTriple result = blockAna.getResults();
	fprintf(stderr, "## Test Result : %i\n", result.unknown);
  }
}
