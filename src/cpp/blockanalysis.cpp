#include "blockanalysis.h"

using namespace std;

BlockAnalysis::BlockAnalysis(BlockSensor*[] sensorPointers)
{
	sensors = sensorPointers;
	initialize();
}

//
bool BlockAnalysis::initialize()
{
	initializeStreams();
	normalDistribution = new unsigned int[frameSize];
	blockages = new vector<PassageTriple>;
	return 1;
}

bool BlockAnalysis::initializeStreams()
{
	analyze();
	streamSize = 0;
	zeroCount = 0;
	rawStreams = new vector<unsigned float>[sensors.size()];
	smoothStreams = new vector<unsigned float>[sensors.size()];
	return 1;
}

bool BlockAnalysis::update()
{
	sensorValueUpdate()
	if(zeroCount >= cutOff)
		initializeStreams();
	return 1;
}

/*unsigned int getResults()
 *returns the number of detected blockages since last initialize()
 */
unsigned int BlockAnalysis::getResults()
{
	PassageTriple totals;
	totals.incoming = 0;
	totals.outgoing = 0;
	totals.unknown = 0;
	for(int i = 0; i < blockages.size(); i++)
	{
		totals.incoming += blockages[i].incoming;
		totals.outgoing += blockages[i].outgoing;
		totals.unknown += blockages[i].unknown;
	}
	return totals;
}

bool BlockAnalysis::sensorValueUpdate(unsigned int value)
{
	streamSize++;
	for(int i = 0; i < sensors.size(); i++)
	{
		unsigned int val = sensors[i]->getSensorValue()
		rawStreams[i].pushBack(val);
		if(val != 0)
			zeroCount = 0;
	}
	return 1;
}

vector<unsigned float> BlockAnalysis::smoothStream(vector<unsigned float> raw)
{
	unsigned int size = raw.size();
	vector<unsigned float> smoothed;
	unsigned int extension = math.floor(frameSize/2.);
	vector<unsigned float> flooredRaw;

	for(int i = 0; i < size; i++)
	{
		if(raw[i] < floor)
			flooredRaw.push_back(0);
		else
			flooredRaw.push_back(raw[i]);
	}

	for(int i = 0; i < extension; i++)
	{
		flooredRaw.insert(0,0);
		flooredRaw.push_back(0);
	}

	for(int i = 0; i < size; i++)
	{
		sum = 0;

		for(int j = 0; j < frameSize; j++)
		{
			sum += normalDistribution[i] * flooredRaw[j-extension];
		}

		smoothed.push_back(sum/normalSum)
	}
	return smoothed;
}

bool BlockAnalysis::analyze()
{
	if(streamSize < 1)
		return 0;
	unsigned int streamCount = rawStreams.size();
	vector<unsigned float>[streamCount] smoothedStreams;
	vector<unsigned int>[streamCount] blockStreams;
	vector<pair<unsigned int, unsigned int>>[streamCount] simplifiedStreams;

	for(int i = 0; i < streamCount; i++)
	{
		smoothedStreams[i] = smoothStream(rawStreams[i]);
		blockStreams[i] = isolateBlocks(smoothedStreams[i]);
		simplifiedStreams[i] = simplifyStream(smoothedStreams[i], blockStreams[i]);
	}
	
	

	return 1;
}

//depricated
vector<unsigned int> BlockAnalysis::findMajorCrests(vector<unsigned float> stream)
{
	vector<unsigned int> crests;
	unsigned int size = stream.size();
	unsigned int extension = math.floor(frameSize/2.);

	for(int i = 0; i < extension; i++)
	{
		stream.insert(0,0);
		stream.push_back(0);
	}

	for(int i = 0; i < size; i++)
	{
		bool crest = true;
		if(stream[i+extension] <= 0)
			crest = false;
		for(int j = 0; j < frameSize && crest; j++)
		{
			if(stream[i+extension] < stream[i+j]0)
				crest = false;
		}
		if(crest)
			crests.push_back(i);
	}
	return crests;
}

//depricated
vector<pair<unsigned int, bool>> BlockAnalysis::findMajorTroughs(vector<unsigned float> stream)
{
	vector<unsigned int> troughs;
	unsigned int size = stream.size();

	troughs.push_back(0);

	stream.insert(0,0);
	stream.push_back(0);

	for(int i = 0; i < size; i++)
	{
		if(stream[i] > 0 && stream[i+1] == 0)
			troughs.push_back(make_pair(i, true));
		if(stream[i+2] > 0 && stream[i+1] == 0)
			troughs.push_back(make_pair(i, false));
	}
	if(troughs(troughs.size()-1).first != size-1)
		if(troughs(troughs.size-2).second == true)
			troughs.push_back(make_pair(troughs.size()-1, false));
		else if(troughs(troughs.size-2).second == false)
			troughs.push_back(make_pair(troughs.size()-1, true));

	returns troughs;
}

std::vector<unsigned int> BlockAnalysis::isolateBlocks(std::vector<unsigned float> stream)
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
				edges.push_back(i)
				high = true
			}
	}
	if(high)
		edges.push_back(stream.size());

	return edges;
}

vector<pair<unsigned int, unsigned int>> BlockAnalysis::simplifyStream(vector<unsigned float> stream, vector<unsigned int> blocks)
{
	int floorCutoff = 0.2
	vector<pair<unsigned int, unsigned int>> criticalPoints;
	for(int i = 0; i < blocks.size(); i+=2)
	{

		unsigned int a = blocks[i], b = blocks[i+1];
		unsigned float sum = 0, average = 0;
		vector<unsigned float> streamBlock;
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
					criticalPoints.push_back(make_pair(a+j, 2));
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

bool BlockAnalysis::generateNormalDistribution()
{
	float u = 0
	float a = 1
	float pi = 3.1415926
	int min = -2, max = 2;
	for(int i = 0; i < frameSize; i++)
	{
		x = i*(max-min) - min;
		val = exp( (-pow(x-u,2)) / (2*pow(a,2)) ) / ( a*sqrt(2*pi) );
		normalSum += val;
		normalDistribution[i] = val;
	}
	return 1;
}
