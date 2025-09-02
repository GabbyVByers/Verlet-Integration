#pragma once

#include "simulation.h"
#include "vec2f.h"

struct cellID
{
	int cellX = 0;
	int cellY = 0;
};

inline cellID getCellIdFromPosition(Simulation& simulation, Vec2f position)
{
	float width = simulation.gridWidth;
	int cellX = (position.x > 0.0f) ? ((position.x + (width * 0.5f)) / width) : ((position.x - (width * 0.5f)) / width);
	int cellY = (position.y > 0.0f) ? ((position.y + (width * 0.5f)) / width) : ((position.y - (width * 0.5f)) / width);
	return { cellX, cellY };
}

struct ballCellKeyPair
{
	int ballIndex = -1;
	unsigned int cellKey = 0;
};

inline unsigned int hashCell(cellID cellId)
{
	unsigned int a = (unsigned int)cellId.cellX * 15823;
	unsigned int b = (unsigned int)cellId.cellY * 9737333;
	return a + b;
}

inline unsigned int keyFromHash(unsigned int hash, int numCells)
{
	return hash % (unsigned int)numCells;
}

inline void buildSpatialPartition(Simulation& simulation)
{
	ballCellKeyPair* ballCellKeys = nullptr;
	ballCellKeys = new ballCellKeyPair[simulation.numBalls];

	int numCells = 1000;

	for (int i = 0; i < simulation.numBalls; i++)
	{
		ballCellKeyPair ballCellKey;
		ballCellKey.ballIndex = i;
		ballCellKey.cellKey = keyFromHash(hashCell(getCellIdFromPosition(simulation, simulation.balls[i].currPos)), numCells);
		ballCellKeys[i] = ballCellKey;
	}



	delete[] ballCellKeys;
}