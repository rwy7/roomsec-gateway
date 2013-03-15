#include 'tailgateanalyzer.h'

TailgateAnalyzer::TailgateAnalyzer()
{
	sessionRunning = false;
}

bool TailgateAnalyzer::setSensor(std::vector<BlockSensor *> sensors)
{
	newSensors.clear();
	for (int i = 0; i < sensors.size(); i++)
	{
		newSensors.push_back(sensors[i]);
	}
	constructBlockAnalyzer();
	return true;
}

bool TailgateAnalyzer::constructBlockAnalyzer()
{
	if (sessionRunning || newSensors.size() == 0)
		return false;
	blockAnalyzer = new BlockAnalyzer(newSensors);
	newSensors.clear();
	return true;
}

bool TailgateAnalyzer::update()
{
	constructBlockAnalyzer();
	if(sessionRunning)
	{
		blockAnalyzer->update();
	}
}

bool TailgateAnalyzer::beginSession()
{
	if (sessionRunning || !blockAnalyzer)
		return false;
	sessionRunning = true;
	blockAnalyzer->beginMonitoringSession();
	return true;
}

bool TailgateAnalyzer::finishSession()
{
	if (!sessionRunning || !blockAnalyzer)
		return false;
	sessionRunning = false;
	blockAnalyzer->endMonitoringSession();
	results = blockAnalyzer->getResults();
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
