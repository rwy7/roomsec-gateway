#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "mockblocksensor.h"
#include "blockanalysis.h"
#include <cstdio>


namespace roomsec {
  using namespace ::testing;

  TEST(BlockAnalysisTest, SmoothStream) {
    MockBlockSensor blockSensor1;

	BlockAnalysis blockAna(&blockSensor1);

    EXPECT_CALL(blockSensor1, getSensorValue())
      .Times(AtLeast(1))
      .WillRepeatedly(Return(11));

	for(int i = 0; i < 10; i++)
		blockAna.update();
	blockAna.initializeStream();

	PassageTriple result = blockAna.getResults();
	fprintf(std::stderr, "## Test Result : %i\n", result.unknown);
  }
}
