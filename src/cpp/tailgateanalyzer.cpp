#include "tailgateanalyzer.h"
#include <log4cxx/logger.h>

namespace roomsec {

static log4cxx::loggerPtr logger = log4cxx::logger::getLogger("roomsec.tailgateanalyzer");

TailgateAnalyzer::TailgateAnalyzer(std::vector<BlockSensor*> sensors, bool debug)
{
	LOG4CXX_DEBUG(logger, "TailgateAnalyzer constructing");
	DEBUG = debug;
	sessionRunning = false;
	setSensors(sensors);
	LOG4CXX_DEBUG(logger, "TailgateAnalyzer construction complete"
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
	LOG4CXX_DEBUG(logger, "TailgateAnalyzer constructBlockAnalyzer()");
	if (sessionRunning || newSensors.size() == 0)
	{
		LOG4CXX_DEBUG(logger, "TailgateAnalyzer constructBlockAnalyzer - failed, you either called it at the wrong time or you forgot to set the sensors");
		if (DEBUG)
			printf("TailgateAnalyzer::constructBlockAnalyzer() - called prematurely; Either session already running or no sensors have been set;\n");
		return false;
	}
	blockAnalyzer = new BlockAnalyzer(newSensors, DEBUG);
	newSensors.clear();
	LOG4CXX_DEBUG(logger, "TailgateAnalyzer constructBlockAnalyzer() - complete");
	if (DEBUG)
		printf("TailgateAnalyzer::constructBlockAnalyzer() - blockAnalyzer initialized;\n");
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
		LOG4CXX_DEBUG(logger, "TailgateAnalyzer beginSession() - failed, you either called it at the wrong time or you forgot to initialize blockAnalyzer");
		if (DEBUG)
			printf("#TailgateAnalyzer::beginSession() called improperly, no changes performed; sessionRunning = true || blockAnalyzer uninitialized;\n");
		return false;
	}
	sessionRunning = true;
	blockAnalyzer->beginMonitoringSession();
	LOG4CXX_DEBUG(logger, "TailgateAnalyzer beginSession() - complete, monitoring session started");
	if (DEBUG)
		printf("TailgateAnalyzer::beginSession() - monitoring session started;\n");
	return true;
}

bool TailgateAnalyzer::finishSession()
{
	LOG4CXX_DEBUG(logger, "TailgateAnalyzer finishSession()");
	if (!sessionRunning)
	{
		LOG4CXX_DEBUG(logger, "TailgateAnalyzer finishSession() - failed, you called it at the wrong time");
		if (DEBUG)
			printf("#TailgateAnalyzer::endSession() called improperly, no changes performed; sessionRunning = false;\n");
		return false;
	}
	sessionRunning = false;
	blockAnalyzer->endMonitoringSession();
	results = blockAnalyzer->getResults();
	LOG4CXX_DEBUG(logger, "TailgateAnalyzer finishSession() - complete, monitoring session ended, results pulled from blockAnalyzer");
	if (DEBUG)
		printf("TailgateAnalyzer::finishSession() - monitoring session ended; Results pulled from blockAnalyzer;\n");
	return true;
}

PassageTriple TailgateAnalyzer::getResults()
{
	return results;
}

}
