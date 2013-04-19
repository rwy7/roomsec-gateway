#include "tailgateanalyzer.h"
#include <log4cxx/logger.h>

namespace roomsec {

  static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("roomsec.tailgateanalyzer");

  TailgateAnalyzer::TailgateAnalyzer(std::vector<BlockSensor*> sensors, bool debug)
  {
    LOG4CXX_DEBUG(logger, "- Constructing");
    DEBUG = debug;
    sessionRunning = false;
    setSensors(sensors);
    LOG4CXX_DEBUG(logger, "- Construction complete");
  }

  bool TailgateAnalyzer::setSensors(std::vector<BlockSensor *> sensors)
  {
    LOG4CXX_DEBUG(logger, "- Setting sensors");
    newSensors.clear();
    for (unsigned int i = 0; i < sensors.size(); i++)
		{
	newSensors.push_back(sensors[i]);
		}
    return constructBlockAnalyzer();
	LOG4CXX_DEBUG(logger, "- Setting sensors");
  }

  bool TailgateAnalyzer::constructBlockAnalyzer()
  {
    LOG4CXX_DEBUG(logger, "- Constructing BlockAnalyzer");
    if (sessionRunning || newSensors.size() == 0)
      {
		LOG4CXX_DEBUG(logger, "- failed, sessionRunning = true || newSensors.size() == 0");
		return false;
      }
    blockAnalyzer = new BlockAnalyzer(newSensors, DEBUG);
    newSensors.clear();
    LOG4CXX_DEBUG(logger, "- Construction complete");
    return true;
  }

  bool TailgateAnalyzer::update()
  {
    constructBlockAnalyzer();
    if(sessionRunning)
      {
	blockAnalyzer->update();
      }
    return true;
  }

  bool TailgateAnalyzer::beginSession()
  {
    if (sessionRunning || !blockAnalyzer)
      {
	LOG4CXX_DEBUG(logger, "- Failed, called improperly, sessionRunning = true || blockAnalyzer = false");
	return false;
      }
    sessionRunning = true;
    blockAnalyzer->beginMonitoringSession();
    LOG4CXX_DEBUG(logger, "- Session started");
    return true;
  }

  bool TailgateAnalyzer::finishSession()
  {
    if (!sessionRunning)
      {
	LOG4CXX_DEBUG(logger, "- Failed, called improperly, sessionRunning = false");
	return false;
      }
    sessionRunning = false;
    blockAnalyzer->endMonitoringSession();
    results = blockAnalyzer->getResults();
    LOG4CXX_DEBUG(logger, "- Session ended, results pulled from blockAnalyzer");
    return true;
  }

  PassageTriple TailgateAnalyzer::getResults()
  {
    return results;
  }

}
