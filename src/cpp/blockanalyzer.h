#ifndef BLOCKANALYZER
#define BLOCKANALYZER

#include "blocksensor.h"
#include <vector>
#include "math.h"
#include <utility>
#include <cstdio>

namespace roomsec{

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
 * Analysis assumes that the vector of sensors is ordered such that the first sensor if the outermost sensor in the gateway.
 * That is, an object passing inward through the gateway will pass the sensors in the same order that the sensors are present
 * in the vector.
 *
 * The class begins creates a digital data stream for each sensor by periodically sampling their values. No analysis of the data is 
 * made until the monitoring session is complete. At the end of the monitoring session, the streams are smoothed and refined.
 *
 * The class then steps through the streams, identifying peaks in the data, and identifying matching peaks across the data streams.
 * From this, the class determines the number of passages through the doorway and the probable direction of each passage.
 *
 * update() is intended to be called periodically within some loop by the controlling class. update() must be called periodically 
 * during the monitoring session for proper data gathering to occur. Calling update() outside of the monitoring session is unnecessary,
 * but doind so will not result in any adverse behavior.
 *
 * A monitoring session is started with startMonitoringSession(). This initializes the data and begins the data collection process. 
 * No data will be sampled from the sensors until the monitoring session is initialized with startMonitoringSession().
 *
 * A monitoring session is ended with endMonitoringSession(). This ends the collection of data, and performs analyis on the collected data.
 *
 * getResults() will return a PassageTriple containing the identified passages from the last monitoring session. getResults() should only be called 
 * after the monitoring session has been ended with endMonitoringSession(), otherwise getResults() will report incomplete data.
 */
class BlockAnalyzer {
	static const unsigned int frameSize = 7, low = 0, high = 1000, floorCutOff = 35, zeroCutOff = 25;
	unsigned int zeroCount, streamSize, streamCount;
	float *normalDistribution;
	float normalSum;
	std::vector<BlockSensor *> sensors;
	std::vector<float> *rawStreams;// *smoothedStreams;
	std::vector<PassageTriple> blockages;
	bool monitoring, firstRun;
	
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
	std::vector<std::pair<unsigned int, float> > simplifyStreams(std::vector<float> stream, std::vector<unsigned int> blocks);

	PassageTriple analyzeStreams(std::vector<std::pair<unsigned int, float> > *simpleStreams);

	/**
	 * @brief Performs analysis on the collected data to determine number and direction of passages through doorway
	 *
	 */
	bool analyze();	

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

	public:
	bool DEBUG;

	/**
	 * @brief 
	 *
	 */
	BlockAnalyzer(std::vector<BlockSensor*> sensorPointers, bool debug = false);

	/**
	 * @brief Primary update method to be called during each iteration of controlling classes main loop
	 *
	 */
	bool update();

	/**
	 * @brief initializes the monitoring session so that the collection of data may begin
	 *
	 */
	bool beginMonitoringSession();

	/**
	 * @brief ends the monitoring session and performs analysis on the collected data
	 *
	 */
	bool endMonitoringSession();

	/**
	 * @brief Gets the accumulated results of all analysis since the last initialization
	 *
	 */
	PassageTriple getResults();
	
};
}
#endif

