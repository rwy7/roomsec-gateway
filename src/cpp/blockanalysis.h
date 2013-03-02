#ifndef BLOCKANALYSIS
#define BLOCKANALYSIS

#include "blocksensor.h"
#include <vector>
#include "math.h"
#include <utility>


struct PassageTriple
{
	unsigned int ingoing, outgoing, unknown;
};

class BlockAnalysis{
	static const unsigned int frameSize = 5, low = 0, high = 1000, floorCutOff = 10, zeroCutOff = 100;
	unsigned int zeroCount, streamSize, streamCount;
	float *normalDistribution;
	float normalSum;
	std::vector<BlockSensor *> sensors;
	std::vector<float> *rawStreams, *smoothedStreams;
	std::vector<PassageTriple> blockages;
	
	public: //temporary exposing internalfunctions for testing purposes
	bool generateNormalDistribution();
	bool sensorValueUpdate();
	std::vector<float> smoothStream(std::vector<float> raw);
	bool analyze();	
	std::vector<unsigned int> isolateBlocks(std::vector<float> stream);
	std::vector<std::pair<unsigned int, unsigned int> > simplifyStream(std::vector<float> stream, std::vector<unsigned int> blocks);
	std::vector<unsigned int> combineStreams(std::vector<std::pair<unsigned int, unsigned int> > streams[]);

	//public:
	BlockAnalysis(std::vector<BlockSensor*> sensorPointers);
	bool initialize();
	bool initializeStreams();
	bool update();
	PassageTriple getResults();
	
};

#endif
