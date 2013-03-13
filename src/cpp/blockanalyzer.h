#ifndef BLOCKANALYZER
#define BLOCKANALYZER

#include "blocksensor.h"
#include <vector>
#include "math.h"
#include <utility>

/**
 * @brief A structure that holds a triple of values representing the number of passages through the doorway
 *
 * ingoing - the number of recorded ingoing passages
 * outgoing - the number of recorded outgoing passages
 * unknown - the estimated number of passages for which direction could not be determined
 */
struct PassageTriple
{
	unsigned int ingoing, outgoing, unknown;
};

/**
 * @brief This class analyzes the readings from the block sensors in order to monitor the number of passages through the doorway
 *
 * The class begins creates a digital data stream for each sensor by periodically sampling their values. No analysis of the data is 
 * made until the monitoring session is complete. At the end of the monitoring session, the streams are smoothed and refined.
 *
 * The class then steps through the streams, identifying peaks in the data, and identifying matching peaks across the data streams.
 * From this, the class determines the number of passages through the doorway and the probable direction of each passage.
 *
 */
class BlockAnalyzer{
	static const unsigned int frameSize = 5, low = 0, high = 1000, floorCutOff = 10, zeroCutOff = 100;
	unsigned int zeroCount, streamSize, streamCount;
	float *normalDistribution;
	float normalSum;
	std::vector<BlockSensor *> sensors;
	std::vector<float> *rawStreams, *smoothedStreams;
	std::vector<PassageTriple> blockages;
	
	/**
	 * @brief Generates a normal distribution of the length frameSize for use in smoothing
	 *
	 */
	bool generateNormalDistribution();

	/**
	 * @brief Grabs the current values from the sensors
	 *
	 */
	bool sensorValueUpdate();

	/**
	 * @brief Performs a simple smoothing algorithm on a sensor value stream
	 *
	 */
	std::vector<float> smoothStream(std::vector<float> raw);

	/**
	 * @brief Identifies blocks of detection from a sensor value stream
	 *
	 */
	std::vector<unsigned int> isolateBlocks(std::vector<float> stream);

	/**
	 * @brief Simplifies a sensor value stream into a series of critical values, eg. local peaks and troughs
	 *
	 */
	std::vector<std::pair<unsigned int, float> > simplifyStream(std::vector<float> stream, std::vector<unsigned int> blocks);


	public:

	/**
	 * @brief 
	 *
	 */
	BlockAnalyzer(std::vector<BlockSensor*> sensorPointers);

	/**
	 * @brief Reinitializes all volatile values
	 *
	 */
	bool initialize();

	/**
	 * @brief Reinitializes the sensor value streams
	 *
	 */
	bool initializeStreams();

	/**
	 * @brief Primary update method to be called during each iteration of controlling classes main loop
	 *
	 */
	bool update();

	/**
	 * @brief Performs analysis on the collected data to determine number and direction of passages through doorway
	 *
	 */
	bool analyze();	

	/**
	 * @brief Gets the accumulated results of all analysis since the last initialization
	 *
	 */
	PassageTriple getResults();
	
};

#endif
