#include "blockanalyzer.h"
#include <log4cxx/logger.h>

using namespace std;

namespace roomsec{

static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("roomsec.blockanalyzer");

BlockAnalyzer::BlockAnalyzer(std::vector<BlockSensor*> sensorPointers, bool debug)
{
	LOG4CXX_DEBUG(logger, "- Constructing");
	DEBUG = debug;
	sensors.clear();
	streamCount = sensorPointers.size();
	for(unsigned int i = 0; i < streamCount; i++)
		sensors.push_back(sensorPointers[i]);
	monitoring = false;
	firstRun = true;
	LOG4CXX_DEBUG(logger, "- Construction complete");
}

//
bool BlockAnalyzer::initialize()
{
	LOG4CXX_DEBUG(logger, "- Beginning initialization");
	initializeStreams();
	normalSum = 0;
	normalDistribution = new float[frameSize];
	generateNormalDistribution();
	blockages.clear();
	LOG4CXX_DEBUG(logger, "- Initialization complete");
	return 1;
}

bool BlockAnalyzer::initializeStreams()
{
	LOG4CXX_DEBUG(logger, "- Beginning stream initialization");
	if( firstRun)
		firstRun = false;
	else
		analyze();
	streamSize = 0;
	zeroCount = 0;
	rawStreams = new vector<float>[sensors.size()];
	LOG4CXX_DEBUG(logger, "- Initialization complete");
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
	{
		LOG4CXX_DEBUG(logger,"- Reinitializing due to long period of dead readings");
		initializeStreams();
	}
	return 1;
}

/*unsigned int getResults()
 *returns the number of detected blockages since last initialize()
 */
PassageTriple BlockAnalyzer::getResults()
{
	LOG4CXX_DEBUG(logger, "- Grabbing Results");
	if(monitoring)
		LOG4CXX_DEBUG(logger, "- called during monitoring session; Possiblity of redundent or incomplete results");
	if (DEBUG && monitoring)
		printf("#BlockAnalyzer::getResult() - called during monitoring session; Possible incomplete results returned;\n");
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

	char buffer[100];
	sprintf(buffer, "- Complete, ingoing=%i, outgoing=%i, unknown=%i", totals.ingoing, totals.outgoing,totals.unknown);
	LOG4CXX_DEBUG(logger, buffer);
	return totals;
}

bool BlockAnalyzer::sensorValueUpdate()
{
	LOG4CXX_TRACE(logger, "- Updating sensor values");
	if (DEBUG)
		printf("#BlockAnalyzer::sensorValueUpdate() - sensors.size() = %3li;", sensors.size());
	streamSize++;
	bool zero = true;
	for(unsigned int i = 0; i < sensors.size(); i++)
	{
		unsigned int val = sensors[i]->getSensorValue();
		rawStreams[i].push_back(val);
		if(val != 0)
			zero = false;
		if (DEBUG)
			printf(" sensors[%2i]->getSensorValue = %3i;", i, val); 
	}
	if (DEBUG)
		printf(" BlockAnalyzer::sensorValueUpdate() - Complete;\n");
	if(zero)
		zeroCount++;
	else
		zeroCount = 0;
	LOG4CXX_TRACE(logger, "- Update complete");
	return 1;
}

vector<float> BlockAnalyzer::smoothStream(vector<float> raw)
{
	LOG4CXX_DEBUG(logger, "- Smoothing raw stream");
	unsigned int size = raw.size();
	vector<float> smoothed;
	unsigned int extension = floor(frameSize/2.);
	vector<float> flooredRaw;

	if (DEBUG)
		printf("#BlockAnalyzer::smoothStream(raw) - stream.size() = %i; extension = %i;\n", size, extension);

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
	LOG4CXX_DEBUG(logger, "- Smoothing complete");
	if (DEBUG)
		printf("#BlockAnalyzer::smoothStream(raw) - Complete;\n");
	return smoothed;
}	

bool BlockAnalyzer::analyze()
{
	char buffer[100];
	sprintf(buffer, "- Beginning analysis, streamSize = %i; streamCount = %i;\n", streamSize, streamCount);
	LOG4CXX_DEBUG(logger, buffer);
	if (DEBUG)
		printf("#BlockAnalyzer::analyze() - streamSize = %i; streamCount = %i;\n", streamSize, streamCount);
	if(streamSize < 1 || streamCount < 1)
	{
		LOG4CXX_DEBUG(logger, "- premature termination; streamSize < 1 or streamCount < 1");
		if (DEBUG)
			printf("#BlockAnalyzer::analyze() - premature termination; streamSize < 1 || streamCount < 1;\n");
		return 0;
	}
	
	vector<float> smoothedStreams[streamCount];
	vector<unsigned int> blockStreams[streamCount];
	vector<pair<unsigned int, float> > simplifiedStreams[streamCount];

	for(unsigned int i = 0; i < streamCount; i++)
	{
		//for(int j = 0; j < rawStreams[i].size(); j++)
		//	printf("rawStreams[%i][%i] = %f;\n", i, j, rawStreams[i][j]);
		smoothedStreams[i] = smoothStream(rawStreams[i]);
		//for(int j = 0; j < smoothedStreams[i].size(); j++)
		//	printf("smoothedStreams[%i][%i] = %f;\n", i, j, smoothedStreams[i][j]);
		blockStreams[i] = isolateBlocks(smoothedStreams[i]);
		simplifiedStreams[i] = simplifyStreams(smoothedStreams[i], blockStreams[i]);
	}

	/*
	unsigned int objs = 0;
	for(unsigned int i = 0; i < simplifiedStreams[0].size(); i++)
	{
		//if (DEBUG)
		//		printf("simplifiedStream[0][%i] == %f;\n", i, simplifiedStreams[0][i].second);
		if(simplifiedStreams[0][i].second >= 2)
			objs++;
	}
	PassageTriple blocks;
	blocks.ingoing = 0;
	blocks.outgoing = 0;
	blocks.unknown = objs;
	blockages.push_back(blocks);
	*/

	blockages.push_back(analyzeStreams(simplifiedStreams));

	LOG4CXX_DEBUG(logger, "BlockAnalyzer analyze() - complete");
	if (DEBUG)
		printf("#BlockAnalyzer::analyze() - Complete;\n");
	return 1;
}

PassageTriple BlockAnalyzer::analyzeStreams(vector<pair<unsigned int, float> > *simpleStreams)
{
	char buffer[100];
	sprintf(buffer, "- Beginning sub analysis, streamCount = %i, stream length = %i", streamCount, simpleStreams[0].size());
	LOG4CXX_DEBUG(logger, buffer);
	if (DEBUG)
		printf("#BlockAnalyzer::analyzeStreams(simpleStreams) - begin analysis;\n");
	//simple implementation using no more than 2 streams
	PassageTriple triple;
	triple.ingoing = 0;
	triple.outgoing = 0;
	triple.unknown = 0;

	for(int i = 0; i < streamCount; i++)
	{
		if(simpleStreams[i].size() < 1)
		{
			LOG4CXX_DEBUG(logger, "- No data to analyse, ending analysis");
			return triple;
		}
	}
	if(streamCount >= 2)
	{
		int index1 = -1, index2 = -1, time = 0;
		vector<pair<unsigned int, unsigned int> > passagePairs;
	
		while(index1 < (int)simpleStreams[0].size() && index2 < (int)simpleStreams[1].size())
		{
			bool found1 = false, found2 = false;
			
			//find next peak in first stream
			while (!found1 && index1 < (int)simpleStreams[0].size())
			{
				index1++;
				if(simpleStreams[0][index1].second >= 2)
					found1 = true;
			}
			//find next peak in second stream
			while (!found2 && index2 < (int)simpleStreams[1].size())
			{
				index2++;
				if(simpleStreams[1][index2].second >= 2)
					found2 = true;
			}

			//add that pair of peaks to the vector
			if(found1 && found2)
			{
				pair<unsigned int, unsigned int> pass = make_pair(simpleStreams[0][index1].first, simpleStreams[1][index2].first);
				passagePairs.push_back(pass);
				if (DEBUG)
					printf("#BlockAnalyzer::analyzeStreams(simpleStreams) - found pair (%3i,%3i);\n", pass.first, pass.second);
			}
		}
		if (passagePairs.size() <= 0)
			if (DEBUG)
				printf("#BlockAnalyzer::analyzeStreams(simpleStreams) - no pairs found;\n");
		//count each pair of peaks as either ingoing or outgoing passage
		for(int i = 0; i < passagePairs.size(); i++)
		{
			int i1 = passagePairs[i].first;
			int i2 = passagePairs[i].second;
			if(i1 < i2)
			{
				triple.ingoing++;
			}
			else if(i1 > i2)
			{
				triple.outgoing++;
			}
			else
				triple.unknown++;
		}
		//count unpaired peaks as unknown passages
		while(index1 < simpleStreams[0].size())
		{
			if(simpleStreams[0][index1].second >= 2)
				triple.unknown++;
			index1++;
		}
		while(index2 < simpleStreams[1].size())
		{
			if(simpleStreams[1][index2].second >= 2)
				triple.unknown++;
			index2++;
		}
	}
	else
	{
		LOG4CXX_DEBUG(logger, "- ABORT ABORT ABANDON SHIP!!!!");
	}
	/*
	else if(streamCount == 1)
	{
		for(int i = 0; i < simpleStreams[0].size(); i++)
			if(simpleStreams[0][i].second >= 2)
				triple.unknown++;
	}
	*/
	LOG4CXX_DEBUG(logger, "- Sub analysis complete");
	return triple;
}


std::vector<unsigned int> BlockAnalyzer::isolateBlocks(std::vector<float> stream)
{
	LOG4CXX_DEBUG(logger, "- Isolating data blocks");
	if (DEBUG)
		printf("#BlockAnalyzer::isolateBlocks(stream) - stream.size() = %lu;\n", stream.size());
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

	LOG4CXX_DEBUG(logger, "- Isolation complete");
	if (DEBUG)
		printf("#BlockAnalyzer::isolateBlocks(stream) - Complete; blockEdges.size() = %lu;\n", blockEdges.size());

	return blockEdges;
}

vector<pair<unsigned int, float> > BlockAnalyzer::simplifyStreams(vector<float> stream, vector<unsigned int> blocks)
{
	char buffer[100];
	sprintf(buffer, "- Simplifying streams, stream.size() = %lu; blocks.size() = %lu;\n", stream.size(), blocks.size());
	LOG4CXX_DEBUG(logger, buffer);
	if (DEBUG)
		printf("#BlockAnalyzer::simplifyStream(stream, blocks) - stream.size() = %lu; blocks.size() = %lu;\n", stream.size(), blocks.size()); 

	float highCutOff = 0.8;
	vector<pair<unsigned int, float> > criticalPoints;
	for(unsigned int i = 0; i < blocks.size(); i+=2)
	{

		unsigned int a = blocks[i], b = blocks[i+1];
		float sum = 0, average = 0;
		vector<float> streamBlock;
		streamBlock.push_back(0);
		for(unsigned int j = a; j < b; j++)
		{
			//printf("stream[%i] = %f;\n", j, stream[j]);
			sum += stream[j];
			streamBlock.push_back(stream[j]);
		}
		streamBlock.push_back(0);

		average = sum/(b-a);
		if (DEBUG)
			printf("#BlockAnalyzer::simplifyStream(stream, blocks) - average = %f; sum = %f; a = %i; b = %i;\n", average, sum, a, b);
		bool high = false;
		bool low = false;
		for(unsigned int j = 0; j < (b-a); j++)
		{
			if(streamBlock[j+1] > streamBlock[j] && streamBlock[j+1] > streamBlock[j+2] && streamBlock[j+1] > average*highCutOff)
			{
				if(!high)
					criticalPoints.push_back(make_pair(a+j, 2 + streamBlock[j+1]));
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

	LOG4CXX_DEBUG(logger, "- Simplification complete");
	if (DEBUG)
	{
		printf("#BlockAnalyzer::simplifyStream(stream, blocks) - Complete; criticalPoints.size() = %lu;\n", criticalPoints.size());
	}

	return criticalPoints;
}

bool BlockAnalyzer::beginMonitoringSession()
{
	if (monitoring)
	{
		LOG4CXX_DEBUG(logger, "- Failed, called improperly; monitoring = true");
		if (DEBUG)
			printf("#BlockAnalyzer::beginMonitoringSession() called improperly; monitoring = true;\n");
		return false;
	}
	
	monitoring = true;
	initialize();
	LOG4CXX_DEBUG(logger, "- Monitoring session started");
	if (DEBUG)
		printf("#BlockAnalyzer::beginMonitoringSession() - monitoring session started;\n");
	return true;
}

bool BlockAnalyzer::endMonitoringSession()
{
	if (!monitoring)
	{
		LOG4CXX_DEBUG(logger, "- Failed, called improperly; monitoring = false");
		if (DEBUG)
			printf("#BlockAnalyzer::endMonitoringSession() called improperly; monitoring = false;\n");
		return false;
	}
	
	monitoring = false;
	this->initializeStreams();

	LOG4CXX_DEBUG(logger, "- Monitoring session ended");
	if (DEBUG)
		printf("#BlockAnalyzer::endMonitoringSession() - monitoring session ended;\n");
	return true;
}

bool BlockAnalyzer::generateNormalDistribution()
{
	float u = 0;
	float a = 0.7;
	int min = -2, max = 2;
	float pi = 3.1415926;
	if (DEBUG)
		printf("#BlockAnalyzer::generateNormalDistribution() - frameSize = %i; u = %f; a = %f; min = %i; max = %i;\n", frameSize, u, a, min, max);
	
	normalDistribution = new float[frameSize];
	for(unsigned int i = 0; i < frameSize; i++)
	{
		float x = (float)(i*(max-min))/(frameSize-1) + min;
		float val = exp( (-pow(x-u,2)) / (2*pow(a,2)) ) / ( a*sqrt(2*pi) );
		normalSum += val;
		normalDistribution[i] = val;
	}
	if (DEBUG)
	{
		printf("#BlockAnalyzer::generateNormalDistribution() - Complete; normalDistribution =");
		for(unsigned int i = 0; i < frameSize; i++)
		{
				printf(" %f;", normalDistribution[i]);
		}
		printf("\n");
	}
	return 1;
}
}
