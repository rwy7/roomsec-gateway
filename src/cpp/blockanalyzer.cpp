#include "blockanalyser.h"

using namespace std;


BlockAnalyzer::BlockAnalyzer(std::vector<BlockSensor*> sensorPointers)
{
	sensors.clear();
	streamCount = sensorPointers.size();
	for(int i = 0; i < streamCount; i++)
		sensors.push_back(sensorPointers[i]);
	initialize();
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
	PassageTriple totals;
	totals.ingoing = 0;
	totals.outgoing = 0;
	totals.unknown = 0;
	for(int i = 0; i < blockages.size(); i++)
	{
		totals.ingoing += blockages[i].ingoing;
		totals.outgoing += blockages[i].outgoing;
		totals.unknown += blockages[i].unknown;
	}
	return totals;
}

bool BlockAnalyzer::sensorValueUpdate()
{
	streamSize++;
	for(int i = 0; i < sensors.size(); i++)
	{
		unsigned int val = sensors[i]->getSensorValue();
		rawStreams[i].push_back(val);
		if(val != 0)
			zeroCount = 0;
	}
	return 1;
}

vector<float> BlockAnalyzer::smoothStream(vector<float> raw)
{
	unsigned int size = raw.size();
	vector<float> smoothed;
	unsigned int extension = floor(frameSize/2.);
	vector<float> flooredRaw;

	for(int i = 0; i < size; i++)
	{
		if(raw[i] < floorCutOff)
			flooredRaw.push_back(0);
		else
			flooredRaw.push_back(raw[i]);
	}

	for(int i = 0; i < extension; i++)
	{
		flooredRaw.insert(flooredRaw.begin(),0);
		flooredRaw.push_back(0);
	}

	for(int i = 0; i < size; i++)
	{
		 int sum = 0;

		for(int j = 0; j < frameSize; j++)
		{
			sum += normalDistribution[i] * flooredRaw[j-extension];
		}

		smoothed.push_back(sum/normalSum);
	}
	return smoothed;
}

bool BlockAnalyzer::analyze()
{
	if(streamSize < 1 || streamCount < 1)
		return 0;
	vector<float> smoothedStreams[streamCount];
	vector<unsigned int> blockStreams[streamCount];
	vector<pair<unsigned int, unsigned int> > simplifiedStreams[streamCount];

	for(int i = 0; i < streamCount; i++)
	{
		smoothedStreams[i] = smoothStream(rawStreams[i]);
		blockStreams[i] = isolateBlocks(smoothedStreams[i]);
		simplifiedStreams[i] = simplifyStream(smoothedStreams[i], blockStreams[i]);
	}

	//TODO - turn this into a real analysis
	//TODO - use multiple streams to determine direction
	unsigned int objs = 0;
	for(int i = 0; i < simplifiedStreams[0].size(); i++)
	{
		if(simplifiedStreams[0][i].second >= 2)
			objs++;
	}
	PassageTriple blocks;
	blocks.ingoing = 0;
	blocks.outgoing = 0;
	blocks.unknown = objs;
	blockages.push_back(blocks);
	

	return 1;
}


std::vector<unsigned int> BlockAnalyzer::isolateBlocks(std::vector<float> stream)
{
	vector<unsigned int> edges;

	bool high = false;

	if(stream[0] > 0)
	{
		edges.push_back(0);
		high = true;
	}

	for(int i = 1; i < stream.size(); i++)
	{
		if(high)
			if(stream[i] == 0 && stream[i-1] > 0){
				edges.push_back(i);
				high = false;
			}
		else
			if(stream[i] > 0 && stream[i-1] == 0)
			{
				edges.push_back(i);
				high = true;
			}
	}
	if(high)
		edges.push_back(stream.size());

	return edges;
}

vector<pair<unsigned int, unsigned int> > BlockAnalyzer::simplifyStream(vector<float> stream, vector<unsigned int> blocks)
{
	int floorCutoff = 0.2;
	vector<pair<unsigned int, unsigned int> > criticalPoints;
	for(int i = 0; i < blocks.size(); i+=2)
	{

		unsigned int a = blocks[i], b = blocks[i+1];
		float sum = 0, average = 0;
		vector<float> streamBlock;
		for(int j = a; j < b; j++)
		{
			sum += stream[j];
			streamBlock.push_back(stream[j]);
		}
		average = sum/(b-a);
		bool high = false;
		bool low = false;
		for(int j = 0; j < (b-a); j++)
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
	return criticalPoints;
}

vector<unsigned int> BlockAnalyzer::combineStreams(vector<pair<unsigned int, unsigned int> > streams[])
{
	vector<unsigned int> combined;
	unsigned int end = streams[0][streams[0].size()-1].first;

	unsigned int val = 0, index = 0;
	
	for(int i = 0; i < end; i++)
	{
		if(i == streams[0][index].first)
			val = streams[0][index].second;
		while(i > streams[0][index].first)
			index++;
		combined.push_back(val);
	}
	for(int j = 1; j < streamCount; j++)
	{
		for(int i = 0; i < end; i++)
		{
			if(i == streams[0][index].first)
				val = streams[0][index].second;
			while(i > streams[0][index].first)
				index++;
			combined[i] = combined[i]*val;
		}
	}
	return combined;
}

bool BlockAnalyzer::generateNormalDistribution()
{
	float u = 0;
	float a = 1;
	float pi = 3.1415926;
	int min = -2, max = 2;
	for(int i = 0; i < frameSize; i++)
	{
		int x = i*(max-min) - min;
		float val = exp( (-pow(x-u,2)) / (2*pow(a,2)) ) / ( a*sqrt(2*pi) );
		normalSum += val;
		normalDistribution[i] = val;
	}
	return 1;
}
