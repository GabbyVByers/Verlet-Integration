
#include "simulation.h"

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
	{
		host_keys.resize(numBalls);
		device_keys.resize(numBalls);

		for (int i = 0; i < numBalls; i++)
		{
			BallCellKeyPair& ballCellKeyPair = ballKeyPairs[i];
			unsigned int ballIndex = ballCellKeyPair.ballIndex;
			unsigned int key = keyFromHash(hashCell(getCellIdFromPosition(balls[ballCellKeyPair.ballIndex].currPos)));
			key = (key << 16);
			host_keys[i] = (key | ballIndex);
		}

		thrust::copy(host_keys.begin(), host_keys.end(), device_keys.begin());
		thrust::sort(device_keys.begin(), device_keys.end());
		thrust::copy(device_keys.begin(), device_keys.end(), host_keys.begin());

		for (int i = 0; i < numBalls; i++)
		{
			unsigned int index = host_keys[i] & 0xFFFF;
			unsigned int key = host_keys[i] >> 16;
			BallCellKeyPair& ballCellKey = ballKeyPairs[i];
			ballCellKey.ballIndex = index;
			ballCellKey.cellKey = key;
		}
	}
	ballKeysProfiler.stop();

	startIndicesProfiler.start();
	{
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
	}
	startIndicesProfiler.stop();
}

