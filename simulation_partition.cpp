
#include "simulation.h"

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/generate.h>
#include <thrust/sort.h>
#include <thrust/copy.h>

#include <iomanip>

CellID Simulation::getCellIdFromPosition(Vec2f position)
{
	int cellX = (position.x > 0.0f) ? ((position.x + (gridWidth * 0.5f)) / gridWidth) : ((position.x - (gridWidth * 0.5f)) / gridWidth);
	int cellY = (position.y > 0.0f) ? ((position.y + (gridWidth * 0.5f)) / gridWidth) : ((position.y - (gridWidth * 0.5f)) / gridWidth);
	return { cellX, cellY };
}

unsigned int Simulation::hashCell(CellID cellId)
{
	unsigned int a = (unsigned int)cellId.cellX * 15823;
	unsigned int b = (unsigned int)cellId.cellY * 9737333;
	return a + b;
}

unsigned int Simulation::keyFromHash(unsigned int hash)
{
	return hash % (unsigned int)numUniqueCellKeys;
}

void Simulation::buildSpatialPartition()
{
	ballKeysProfiler.start();

	//Profiler profiler_1;
	//Profiler profiler_2;
	//Profiler profiler_3;

	//profiler_1.start();
	thrust::host_vector<unsigned int> host_keys;
	host_keys.clear();

	for (int i = 0; i < numBalls; i++)
	{
		Ball& ball = balls[i];
		unsigned int ballIndex = i;
		unsigned int key = keyFromHash(hashCell(getCellIdFromPosition(ball.currPos)));
		key = (key << 16);
		host_keys.push_back((unsigned int)(key | ballIndex));
	}
	//profiler_1.stop();
	//std::cout << std::fixed << std::setprecision(4);
	//std::cout << profiler_1.time() << " ";

	//profiler_2.start();
	thrust::device_vector<unsigned int> device_keys = host_keys;
	thrust::sort(device_keys.begin(), device_keys.end());
	thrust::copy(device_keys.begin(), device_keys.end(), host_keys.begin());
	//profiler_2.stop();
	//std::cout << profiler_2.time() << " ";

	//profiler_3.start();
	for (int i = 0; i < numBalls; i++)
	{
		unsigned int index = host_keys[i] & 0xFFFF;
		unsigned int key = host_keys[i] >> 16;
		BallCellKeyPair& ballCellKey = ballKeyPairs[i];
		ballCellKey.ballIndex = index;
		ballCellKey.cellKey = key;
	}
	//profiler_3.stop();
	//std::cout << profiler_3.time() << "\n";

	//int numLoops = 0;
	//while (true)
	//{
	//	numLoops++;
	//	bool isSorted = true;
	//	for (int i = 0; i < (numBalls - 1); i++)
	//	{
	//		BallCellKeyPair& curr = ballKeyPairs[i];
	//		BallCellKeyPair& next = ballKeyPairs[i + 1];
	//		if (curr.cellKey > next.cellKey)
	//		{
	//			isSorted = false;
	//			BallCellKeyPair temp = curr;
	//			curr = next;
	//			next = temp;
	//		}
	//	}
	//
	//	if (isSorted)
	//		break;
	//}
	ballKeysProfiler.stop();

	//std::cout << numLoops << '\n';

	startIndicesProfiler.start();
	for (int i = 0; i < numUniqueCellKeys; i++)
	{
		CellProperties cellProperty;
		startIndices[i] = cellProperty;
	}

	for (int i = 0; i < numBalls; i++)
	{
		BallCellKeyPair& curr = ballKeyPairs[i];
		CellProperties& cellProperty = startIndices[curr.cellKey];
		if (cellProperty.size == 0)
			cellProperty.startIndex = i;
		cellProperty.size++;
	}
	startIndicesProfiler.stop();
}

