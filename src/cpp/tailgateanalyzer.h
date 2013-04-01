#ifndef TAILGATEANALYZER
#define TAILGATEANALYZER

#include "blockanalyzer.h"

namespace roomsec {

class TailgateAnalyzer{
	BlockAnalyzer* blockAnalyzer;
	PassageTriple results;
	std::vector<BlockSensor *> newSensors;
	bool sessionRunning;

	/**
	 * @brief Attempts to construct the block analyzer
	 *
	 * Constructs the block analyzer if all required preconditions are met:
	 *  - a monitoring session is not currently taking place
	 *  - new sensor values have been specified with setSensors(std::vector<BlockSensor *> sensors);
	 *
	 * @return bool indicating the block analyzer was constructed without incident
	 */
	bool constructBlockAnalyzer();


public:
	bool DEBUG;

	/**
	 * @brief 
	 */
	TailgateAnalyzer(std::vector<BlockSensor*> sensors, bool debug = false);

	/**
	 * @brief Specifies the block sensor instances to be monitored
	 * 
	 * The method can safely be called at any point during execution. The old sensor values will
	 * not be overwritten during a monitoring session. The new sensor values will be stores and used
	 * to overwrite the old values once the monitoring session ends.
	 *
     * @param sensors : std::vector<BlockSensor *> A vector of pointers to block sensor instances
	 *
	 * @return bool indicating the method executed without incident
     */		
	bool setSensors(std::vector<BlockSensor*> sensors);


	/**
	 * @brief Primary update function to be called during each execution of the controlling classes main loop
	 *
	 */
	bool update();

	/**
	 * @brief Attempts to start a sensor monitoring session
	 *
	 * Starts a monitoring session if the block analyzer is ready.
	 *  
	 * @return bool indicating that a monitoring session has started
	 */
	bool beginSession();

	/**
	 * @brief Ends the monitoring session, and retreives the results
	 *
	 * Ends the monitoring session, analyzes the collected data and returns the results of the analysis.
	 *  
	 * @return bool
	 */
	bool finishSession();

	/**
	 * @brief A method to retrive a PassageTriple representing the passages observed in the last monitoring session
	 *
	 * @return PassageTriple a triple of values denoting the inward, outward, and directionless passages observed
	 */
	PassageTriple getResults();

};
}
#endif

