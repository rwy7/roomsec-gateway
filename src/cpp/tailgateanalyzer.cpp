#include "tailgateanalyzer.h"
#include <log4cxx/logger.h>

namespace roomsec {

  static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("roomsec.tailgateanalyzer");

  TailgateAnalyzer::TailgateAnalyzer(std::vector<BlockSensor*> sensors, bool debug)
  {
    LOG4CXX_DEBUG(logger, "TailgateAnalyzer constructing");
    DEBUG = debug;
    sessionRunning = false;
    setSensors(sensors);
    LOG4CXX_DEBUG(logger, "TailgateAnalyzer construction complete");
  }

  bool TailgateAnalyzer::setSensors(std::vector<BlockSensor *> sensors)
  {
    LOG4CXX_DEBUG(logger, "TailgateAnalyzer setSensors(sensors)");
    newSensors.clear();
    for (unsigned int i = 0; i < sensors.size(); i++)
      {
	newSensors.push_back(sensors[i]);
      }
    return constructBlockAnalyzer();
  }

  bool TailgateAnalyzer::constructBlockAnalyzer()
  {
    LOG4CXX_DEBUG(logger, "Enter");
    if (sessionRunning || newSensors.size() == 0)
      {
	LOG4CXX_DEBUG(logger, "failed, you either called it at the wrong time or you forgot to set the sensors");
	return false;
      }
    blockAnalyzer = new BlockAnalyzer(newSensors, DEBUG);
    newSensors.clear();
    LOG4CXX_DEBUG(logger, "Complete");
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
    LOG4CXX_DEBUG(logger, "TailgateAnalyzer beginSession()");
    if (sessionRunning || !blockAnalyzer)
      {
	LOG4CXX_DEBUG(logger, "failed, you either called it at the wrong time or you forgot to initialize blockAnalyzer");
	return false;
      }
    sessionRunning = true;
    blockAnalyzer->beginMonitoringSession();
    LOG4CXX_DEBUG(logger, "Complete, monitoring session started");
    return true;
  }

  bool TailgateAnalyzer::finishSession()
  {
    LOG4CXX_DEBUG(logger, "TailgateAnalyzer finishSession()");
    if (!sessionRunning)
      {
	LOG4CXX_DEBUG(logger, "Failed, you called it at the wrong time");
	return false;
      }
    sessionRunning = false;
    blockAnalyzer->endMonitoringSession();
    results = blockAnalyzer->getResults();
    LOG4CXX_DEBUG(logger, "Complete, monitoring session ended, results pulled from blockAnalyzer");
    return true;
  }

  PassageTriple TailgateAnalyzer::getResults()
  {
    return results;
  }

}
