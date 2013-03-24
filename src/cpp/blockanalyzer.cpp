#include "blockanalyzer.h"

using namespace std;

namespace roomsec{

BlockAnalyzer::BlockAnalyzer(std::vector<BlockSensor*> sensorPointers, bool debug)
{
	DEBUG = debug;
	sensors.clear();
	streamCount = sensorPointers.size();
	for(unsigned int i = 0; i < streamCount; i++)
		sensors.push_back(sensorPointers[i]);
	monitoring = false;
}

//
bool BlockAnalyzer::initialize()
{
	
	initializeStreams();
	normalSum = 0;
	normalDistribution = new float[frameSize];
	blockages.clear();
	return 1;
}

bool BlockAnalyzer::initializeStreams()
{
	analyze();
	streamSize = 0;
	zeroCount = 0;
	rawStreams = new vector<float>[sensors.size()];
	smoothedStreams = new vector<float>[sensors.size()];
	return 1;
}

bool BlockAnalyzer::update()
{
	if(!monitoring)
		return 0;
	if(streamCount < 1)
		return 0;
	sensorValueUpdate();
	if(zeroCount >= zeroCutOff)
		initializeStreams();
	return 1;
}

/*unsigned int getResults()
 *returns the number of detected blockages since last initialize()
 */
PassageTriple BlockAnalyzer::getResults()
{
	if (DEBUG && monitoring)
		printf("getResult() - called during monitoring session; Possible incomplete results returned;\n");
	PassageTriple totals;
	totals.ingoing = 0;
	totals.outgoing = 0;
	totals.unknown = 0;
	for(unsigned int i = 0; i < blockages.size(); i++)
	{
		totals.ingoing += blockages[i].ingoing;
		totals.outgoing += blockages[i].outgoing;
		totals.unknown += blockages[i].unknown;
	}
	return totals;
}

bool BlockAnalyzer::sensorValueUpdate()
{
	if (DEBUG)
		printf("sensorValueUpdate() - sensors.size() = %li;", sensors.size());
	streamSize++;
	bool zero = true;
	for(unsigned int i = 0; i < sensors.size(); i++)
	{
		unsigned int val = sensors[i]->getSensorValue();
		rawStreams[i].push_back(val);
		if(val != 0)
			zero = false;
		if (DEBUG)
			printf(" sensors[%i]->getSensorValue = val;", val); 
	}
	if (DEBUG)
		printf(" sensorValueUpdate() - Complete;\n");
	if(zero)
		zeroCount++;
	else
		zeroCount = 0;
	return 1;
}

vector<float> BlockAnalyzer::smoothStream(vector<float> raw)
{
	unsigned int size = raw.size();
	vector<float> smoothed;
	unsigned int extension = floor(frameSize/2.);
	vector<float> flooredRaw;

	if (DEBUG)
		printf("smoothStream(raw) - stream.size() = %i; extension = %i;\n", size, extension);

	for(unsigned int i = 0; i < size; i++)
	{
		if(raw[i] < floorCutOff)
			flooredRaw.push_back(0);
		else
			flooredRaw.push_back(raw[i]);
	}

	for(unsigned int i = 0; i < extension; i++)
	{
		flooredRaw.insert(flooredRaw.begin(),flooredRaw.front());
		flooredRaw.push_back(flooredRaw.back());
	}

	for(unsigned int i = 0; i < size; i++)
	{
		
		float sum = 0;

		for(unsigned int j = 0; j < frameSize; j++)
		{
			sum += normalDistribution[j] * flooredRaw[i+j];
		}
		smoothed.push_back(sum/normalSum);
	}
	if (DEBUG)
		printf("smoothStream(raw) - Complete;\n");
	return smoothed;
}	

bool BlockAnalyzer::analyze()
{
	if (DEBUG)
		printf("analyze() - streamSize = %i; streamCount = %i;\n", streamSize, streamCount);
	if(streamSize < 1 || streamCount < 1)
	{
		if (DEBUG)
			printf("analyze() - premature termination; streamSize < 1 || streamCount < 1;\n");
		return 0;
	}

	vector<float> smoothedStreams[streamCount];
	vector<unsigned int> blockStreams[streamCount];
	vector<pair<unsigned int, float> > simplifiedStreams[streamCount];

	for(unsigned int i = 0; i < streamCount; i++)
	{
		smoothedStreams[i] = smoothStream(rawStreams[i]);
		blockStreams[i] = isolateBlocks(smoothedStreams[i]);
		simplifiedStreams[i] = simplifyStreams(smoothedStreams[i], blockStreams[i]);
	}

	//TODO - turn this into a real analysis
	//TODO - use multiple streams to determine direction
	unsigned int objs = 0;
	for(unsigned int i = 0; i < simplifiedStreams[0].size(); i++)
	{
		if(simplifiedStreams[0][i].second >= 2)
			objs++;
	}
	PassageTriple blocks;
	blocks.ingoing = 0;
	blocks.outgoing = 0;
	blocks.unknown = objs;
	blockages.push_back(blocks);
	
	if (DEBUG)
		printf("analyze() - Complete;\n");
	return 1;
}


std::vector<unsigned int> BlockAnalyzer::isolateBlocks(std::vector<float> stream)
{
	if (DEBUG)
		printf("isolateBlocks(stream) - stream.size() = %lu;\n", stream.size());
	vector<unsigned int> blockEdges;

	bool high = false;

	if(stream[0] > 0)
	{
		blockEdges.push_back(0);
		high = true;
	}

	for(unsigned int i = 1; i < stream.size(); i++)
	{
		if(high)
                {
			if(stream[i] == 0 && stream[i-1] > 0){
				blockEdges.push_back(i);
				high = false;
			}
                }
		else
                {
			if(stream[i] > 0 && stream[i-1] == 0)
			{
				blockEdges.push_back(i);
				high = true;
			}
                }
	}
	if(high)
		blockEdges.push_back(stream.size());

	if (DEBUG)
		printf("isolateBlocks(stream) - Complete; blockEdges.size() = %lu;\n", blockEdges.size());

	return blockEdges;
}

vector<pair<unsigned int, float> > BlockAnalyzer::simplifyStreams(vector<float> stream, vector<unsigned int> blocks)
{
	if (DEBUG)
		printf("simplifyStream(stream, blocks) - stream.size() = %lu; blocks.size() = %lu;\n", stream.size(), blocks.size()); 

	float floorCutoff = 0.2;
	vector<pair<unsigned int, float> > criticalPoints;
	for(unsigned int i = 0; i < blocks.size(); i+=2)
	{

		unsigned int a = blocks[i], b = blocks[i+1];
		float sum = 0, average = 0;
		vector<float> streamBlock;
		for(unsigned int j = a; j < b; j++)
		{
			sum += stream[j];
			streamBlock.push_back(stream[j]);
		}
		average = sum/(b-a);
		bool high = false;
		bool low = false;
		for(unsigned int j = 0; j < (b-a); j++)
		{
			if(streamBlock[j] > average*floorCutoff)
			{
				if(!high)
					criticalPoints.push_back(make_pair(a+j, 2 + streamBlock[j]));
				high = true;
				low = false;
			}
			else
			{
				if(!low)
					criticalPoints.push_back(make_pair(a+j, 1));
				high = false;
				low = true;
			}
		}
		criticalPoints.push_back(make_pair(blocks[i+1], 0));
	}
	if (DEBUG)
	{
		printf("simplifyStream(stream, blocks) - Complete; criticalPoints.size() = %lu;\n", criticalPoints.size());
	}

	return criticalPoints;
}

bool BlockAnalyzer::beginMonitoringSession()
{
	if (monitoring)
	{
		if (DEBUG)
			printf("beginMonitoringSession() called improperly; monitoring = true;\n");
		return false;
	}
	if (DEBUG)
		printf("beginMonitoringSession() - monitoring session started;\n");
	monitoring = true;
	initialize();
	return true;
}

bool BlockAnalyzer::endMonitoringSession()
{
	if (!monitoring)
	{
		if (DEBUG)
			printf("endMonitoringSession() called improperly; monitoring = false;\n");
		return false;
	}
	if (DEBUG)
		printf("endMonitoringSession() - monitoring session ended;\n");
	monitoring = false;
	this->initializeStreams();
	return true;
}

bool BlockAnalyzer::generateNormalDistribution()
{
	float u = 0;
	float a = 1.1;
	int min = -2, max = 2;
<<<<<<< HEAD

	if (DEBUG)
		printf("generateNormalDistribution() - frameSize = %i; u = %f; a = %f; min = %i; max = %i;\n", frameSize, u, a, min, max);

	for(unsigned int i = 0; i < frameSize; i++)
=======
	float pi = 3.1415926;
	if (DEBUG)
		printf("generateNormalDistribution() - frameSize = %i; u = %f; a = %f; min = %i; max = %i;\n", frameSize, u, a, min, max);
	
	normalDistribution = new float[frameSize];
	for(int i = 0; i < frameSize; i++)
>>>>>>> bf9745734720438843f81f3814e1f39c5ca15ebe
	{
		float x = (float)(i*(max-min))/(frameSize-1) + min;
		float val = exp( (-pow(x-u,2)) / (2*pow(a,2)) ) / ( a*sqrt(2*pi) );
		normalSum += val;
		normalDistribution[i] = val;
	}
	if (DEBUG)
	{
		printf("generateNormalDistribution() - Complete; normalDistribution =");
		for(unsigned int i = 0; i < frameSize; i++)
		{
				printf(" %f;", normalDistribution[i]);
		}
		printf("\n");
	}
	return 1;
}
}
