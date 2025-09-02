#pragma once

#include "simulation.h"
#include "vec2f.h"

inline Vec2f getVelocity(Ball& ball, float dt)
{
	Vec2f nextPos = (ball.currPos * 2.0f) - ball.prevPos + (ball.acceleration * (dt * dt));
	return (nextPos - ball.prevPos) / (2.0f * dt);
}

inline void setVelocity(Ball& ball, Vec2f& velocity, float dt)
{
	ball.prevPos = ball.currPos - (velocity * dt) + (ball.acceleration * (0.5f * dt * dt));
}

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

inline void walls(Simulation& simulation)
{
	Ball* balls = simulation.balls;
	float dt = simulation.sub_dt;

	for (int i = 0; i < simulation.numBalls; i++)
	{
		Ball& ball = balls[i];

		if (ball.currPos.y - ball.radius < -1.0f) // floor
		{
			Vec2f velocity = getVelocity(ball, dt);
			velocity.y = fabs(velocity.y);
			velocity = velocity * simulation.bounceDampening;
			ball.currPos.y = -1.0f + ball.radius;
			setVelocity(ball, velocity, dt);
		}

		if (ball.currPos.y + ball.radius > 1.0f) // ceiling
		{
			Vec2f velocity = getVelocity(ball, dt);
			velocity.y = -1.0f * fabs(velocity.y);
			velocity = velocity * simulation.bounceDampening;
			ball.currPos.y = 1.0f - ball.radius;
			setVelocity(ball, velocity, dt);
		}

		if (ball.currPos.x + ball.radius > simulation.max_u) // right wall
		{
			Vec2f velocity = getVelocity(ball, dt);
			velocity.x = -1.0f * fabs(velocity.x);
			velocity = velocity * simulation.bounceDampening;
			ball.currPos.x = simulation.max_u - ball.radius;
			setVelocity(ball, velocity, dt);
		}

		if (ball.currPos.x - ball.radius < (-1.0 * simulation.max_u)) // left wall
		{
			Vec2f velocity = getVelocity(ball, dt);
			velocity.x = fabs(velocity.x);
			velocity = velocity * simulation.bounceDampening;
			ball.currPos.x = (-1.0 * simulation.max_u) + ball.radius;
			setVelocity(ball, velocity, dt);
		}
	}
}

inline void collisions(Simulation& simulation)
{
	Ball* balls = simulation.balls;
	float dt = simulation.sub_dt;
	float dampening = simulation.bounceDampening;

	for (int i = 0; i < simulation.numBalls; i++)
	{
		Ball& ball = balls[i];

		for (int j = 0; j < simulation.numBalls; j++)
		{
			if (i == j) continue;
			Ball& otherBall = balls[j];

			Vec2f difference = ball.currPos - otherBall.currPos;
			float distance = length(difference);
			float radiuses = ball.radius + otherBall.radius;
			if (distance > radiuses) continue;
			if (distance == 0.0f) continue;

			Vec2f V1 = getVelocity(ball, dt);
			Vec2f V2 = getVelocity(otherBall, dt);
			Vec2f P1 = ball.currPos;
			Vec2f P2 = otherBall.currPos;
			float M1 = ball.mass;
			float M2 = otherBall.mass;
			float Mass = M1 + M2;
			
			Vec2f newV1 = V1 - ((P1 - P2) * (((2.0f * M2) / Mass) * (dot(V1 - V2, P1 - P2) / lengthSquared(P1 - P2))));
			Vec2f newV2 = V2 - ((P2 - P1) * (((2.0f * M1) / Mass) * (dot(V2 - V1, P2 - P1) / lengthSquared(P2 - P1))));
			newV1 = newV1 * dampening;
			newV2 = newV2 * dampening;
			
			setVelocity(ball, newV1, dt);
			setVelocity(otherBall, newV2, dt);

			float overlap = radiuses - distance;
			normalize(difference);
			Vec2f offset = difference * overlap;

			ball.currPos = ball.currPos + (offset * (M2 / Mass) * 1.1f);
			ball.prevPos = ball.prevPos + (offset * (M2 / Mass) * 1.1f);
			otherBall.currPos = otherBall.currPos - (offset * (M1 / Mass) * 1.1f);
			otherBall.prevPos = otherBall.prevPos - (offset * (M1 / Mass) * 1.1f);
		}
	}
}

inline void update(Simulation& simulation)
{
	for (int i = 0; i < simulation.stepsPerFrame; i++)
	{
		step(simulation);
		walls(simulation);
		collisions(simulation);
	}
}

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

