#pragma once

#include "simulation.h"
#include "vec2f.h"

inline void step(Simulation& simulation)
{
	Ball* balls = simulation.balls;
	float dt = simulation.sub_dt;

	for (int i = 0; i < simulation.numBalls; i++)
	{
		Ball& ball = balls[i];

		Vec2f newPos = (ball.currPos * 2.0f) - ball.prevPos + (ball.acceleration * (dt * dt));
		ball.prevPos = ball.currPos;
		ball.currPos = newPos;
	}
}

inline void update(Simulation& simulation)
{
	for (int i = 0; i < simulation.stepsPerFrame; i++)
	{
		step(simulation);
	}
}

