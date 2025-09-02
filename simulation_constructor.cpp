
#include "simulation.h"

Simulation::Simulation()
{
	ballKeyPairs = new BallCellKeyPair[numBalls];
	startIndices = new CellProperties[numUniqueCellKeys];

	balls = new Ball[numBalls];
	for (int i = 0; i < numBalls; i++)
	{
		Ball ball;
		ball.currPos = randomVec2f(-1.0f, 1.0f);
		Vec2f initVelocity = randomVec2f(-1.0f, 1.0f);
		normalize(initVelocity);
		ball.prevPos = ball.currPos + (initVelocity * 0.001f);
		ball.color = randomColor3f();
		ball.radius = gridWidth / 2.0f;
		ball.mass = ball.radius * ball.radius;
		ball.acceleration = { 0.0f, -50.0f };
		balls[i] = ball;
	}

	sub_dt = deltaTime / stepsPerFrame;
}

Simulation::~Simulation()
{
	delete[] ballKeyPairs;
	delete[] startIndices;
	delete[] balls;
}

