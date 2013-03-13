#ifndef TAILGATEANALYZER
#define TAILGATEANALYZER

#include 'blockanalyzer.h'

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
	/**
	 * @brief 
	 */
	TailgateAnalyzer();

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
	bool setSensors(std::vector<BlockSensor *> sensors);

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
	 * @brief Ends the monitoring session, and retreivs the results
	 *
	 * Ends the monitoring session, analyzes the collected data and returns the results of the analysis.
	 *  
	 * @return PassageTriple indicating how many objects passed through the gateway and in which direction.
	 */
	PassageTriple finishSession();

	/**
	 * @brief Analyzes the results from the last monitoring session to detect tailgating
	 *
	 *  
	 * @return bool
	 */
	bool analyzeResults();

}

#endif

