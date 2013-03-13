#ifndef BLOCKANALYZER
#define BLOCKANALYZER

#include "blocksensor.h"
#include <vector>
#include "math.h"
#include <utility>


struct PassageTriple
{
	unsigned int ingoing, outgoing, unknown;
};

class BlockAnalyzer{
	static const unsigned int frameSize = 5, low = 0, high = 1000, floorCutOff = 10, zeroCutOff = 100;
	unsigned int zeroCount, streamSize, streamCount;
	float *normalDistribution;
	float normalSum;
	std::vector<BlockSensor *> sensors;
	std::vector<float> *rawStreams, *smoothedStreams;
	std::vector<PassageTriple> blockages;
	
	/**
	 *
	 *
	 */
	bool generateNormalDistribution();

	/**
	 *
	 *
	 */
	bool sensorValueUpdate();

	/**
	 *
	 *
	 */
	std::vector<float> smoothStream(std::vector<float> raw);

	/**
	 *
	 *
	 */
	std::vector<unsigned int> isolateBlocks(std::vector<float> stream);

	/**
	 *
	 *
	 */
	std::vector<std::pair<unsigned int, float> > simplifyStream(std::vector<float> stream, std::vector<unsigned int> blocks);


	public:

	/**
	 * @ 
	 *
	 */
	BlockAnalyzer(std::vector<BlockSensor*> sensorPointers);

	/**
	 *
	 *
	 */
	bool initialize();

	/**
	 *
	 *
	 */
	bool initializeStreams();

	/**
	 *
	 *
	 */
	bool update();

	/**
	 *
	 *
	 */
	bool analyze();	

	/**
	 *
	 *
	 */
	PassageTriple getResults();
	
};

#endif
