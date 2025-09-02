
#include "simulation.h"

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
	for (int i = 0; i < numBalls; i++)
	{
		BallCellKeyPair& ballCellKey = ballKeyPairs[i];
		ballCellKey.cellKey = keyFromHash(hashCell(getCellIdFromPosition(balls[ballCellKey.ballIndex].currPos)));
	}

	while (true)
	{
		bool isSorted = true;
		for (int i = 0; i < (numBalls - 1); i++)
		{
			BallCellKeyPair& curr = ballKeyPairs[i];
			BallCellKeyPair& next = ballKeyPairs[i + 1];
			if (curr.cellKey > next.cellKey)
			{
				isSorted = false;
				BallCellKeyPair temp = curr;
				curr = next;
				next = temp;
			}
		}

		if (isSorted)
			break;
	}
	ballKeysProfiler.stop();

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

