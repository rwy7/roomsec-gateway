#include "tailgateanalyzer.h"

namespace roomsec {

TailgateAnalyzer::TailgateAnalyzer(bool debug)
{
	DEBUG = debug;
	sessionRunning = false;
}

bool TailgateAnalyzer::setSensors(std::vector<BlockSensor *> sensors)
{
	
	newSensors.clear();
	for (unsigned int i = 0; i < sensors.size(); i++)
	{
		newSensors.push_back(sensors[i]);
	}
	constructBlockAnalyzer();
	return true;
}

bool TailgateAnalyzer::constructBlockAnalyzer()
{
	if (sessionRunning || newSensors.size() == 0)
	{
		if (DEBUG)
			printf("TailgateAnalyzer::constructBlockAnalyzer() - called prematurely; Either session already running or no sensors have been set;\n");
		return false;
	}
	blockAnalyzer = new BlockAnalyzer(newSensors, DEBUG);
	newSensors.clear();
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
	if (sessionRunning || !blockAnalyzer)
		return false;
	sessionRunning = true;
	blockAnalyzer->beginMonitoringSession();
	if (DEBUG)
		printf("TailgateAnalyzer::beginSession() - monitoring session started;\n");
	return true;
}

bool TailgateAnalyzer::finishSession()
{
	if (!sessionRunning || !blockAnalyzer)
		return false;
	sessionRunning = false;
	blockAnalyzer->endMonitoringSession();
	results = blockAnalyzer->getResults();
	if (DEBUG)
		printf("TailgateAnalyzer::finishSession() - monitoring session ended; Results pulled from blockAnalyzer;\n");
	return true;
}

bool TailgateAnalyzer::analyze()
{
	//if results.ingoing > 1
		//tailgate = true
	//else if (results.ingoing + results.unkown > 1)
		//tailgate = possible
	return false;
}

}
