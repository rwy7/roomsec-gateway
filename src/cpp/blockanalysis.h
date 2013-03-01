#ifndef BLOCKANALYSIS
#define BLOCKANALYSIS

#include "blocksensor.h"
#include <std::vector>
#include "math.h"
#include <tuple>
#include <pair>
#include <utility>


struct PassageTriple
{
	unsigned int ingoing, outgoing, unknown;
};

class BlockAnalysis{
	const unsigned int frameSize = 5, low = 0, high = 1000, floor = 10, cutOff = 100;
	unsigned int zeroCount = 0, streamSize = 0;
	unsigned float[] normalDistribution;
	unsigned float normalSum = 0;
	BlockSensor*[] sensors;
	std::vector<unsigned float>[] rawStreams, smoothedStreams;
	std::vector<PassageTriple> blockages;
	
	bool generateNormalDistribution();
	bool sensorValueUpdate(unsigned float value);
	std::vector<unsigned float> smoothStream(std::vector<unsigned float> raw);
	bool analyze();	
	std::vector<unsigned int> isolateBlocks(std::vector<unsigned float> stream);
	std::vector<std::pair<unsigned int, unsigned int>> simplifyStream(std::vector<unsigned float> stream, std::vector<unsigned int> blocks);
	std::vector<unsigned int> combineStreams(std::vector<std::pair<unsigned int, unsigned int>>[] streams);
	std::vector<
	//depricated
	std::vector<unsigned int> findMajorCrests(std::vector<unsigned float> stream);
	//depricated
	std::vector<std::pair<unsigned int, bool>> findMajorTroughs(std::vector<unsigned float> stream);

	public:
	BlockAnalysis(BlockSensor*[] sensorPointers);
	bool initialize();
	bool initializeStreams();
	bool update();
	PassageTriple getResults();
	
};

#endif
