
#include "simulation.h"

Vec2f Simulation::getVelocity(Ball& ball)
{
	Vec2f nextPos = (ball.currPos * 2.0f) - ball.prevPos + (ball.acceleration * (sub_dt * sub_dt));
	return (nextPos - ball.prevPos) / (2.0f * sub_dt);
}

void Simulation::setVelocity(Ball& ball, Vec2f& velocity)
{
	ball.prevPos = ball.currPos - (velocity * sub_dt) + (ball.acceleration * (0.5f * sub_dt * sub_dt));
}

void Simulation::step()
{
	for (int i = 0; i < numBalls; i++)
	{
		Ball& ball = balls[i];

		Vec2f newPos = (ball.currPos * 2.0f) - ball.prevPos + (ball.acceleration * (sub_dt * sub_dt));
		ball.prevPos = ball.currPos;
		ball.currPos = newPos;
	}
}

void Simulation::walls()
{
	for (int i = 0; i < numBalls; i++)
	{
		Ball& ball = balls[i];

		if (ball.currPos.y - ball.radius < -1.0f) // floor
		{
			Vec2f velocity = getVelocity(ball);
			velocity.y = fabs(velocity.y);
			velocity = velocity * bounceDampening;
			ball.currPos.y = -1.0f + ball.radius;
			setVelocity(ball, velocity);
		}

		if (ball.currPos.y + ball.radius > 1.0f) // ceiling
		{
			Vec2f velocity = getVelocity(ball);
			velocity.y = -1.0f * fabs(velocity.y);
			velocity = velocity * bounceDampening;
			ball.currPos.y = 1.0f - ball.radius;
			setVelocity(ball, velocity);
		}

		if (ball.currPos.x + ball.radius > max_u) // right wall
		{
			Vec2f velocity = getVelocity(ball);
			velocity.x = -1.0f * fabs(velocity.x);
			velocity = velocity * bounceDampening;
			ball.currPos.x = max_u - ball.radius;
			setVelocity(ball, velocity);
		}

		if (ball.currPos.x - ball.radius < (-1.0 * max_u)) // left wall
		{
			Vec2f velocity = getVelocity(ball);
			velocity.x = fabs(velocity.x);
			velocity = velocity * bounceDampening;
			ball.currPos.x = (-1.0 * max_u) + ball.radius;
			setVelocity(ball, velocity);
		}
	}
}

void Simulation::resolveCollision(Ball& ball, Ball& otherBall)
{
	Vec2f difference = ball.currPos - otherBall.currPos;
	float distance = length(difference);
	float radiuses = ball.radius + otherBall.radius;
	if (distance > radiuses) return;
	if (distance == 0.0f) return;

	Vec2f V1 = getVelocity(ball);
	Vec2f V2 = getVelocity(otherBall);
	Vec2f P1 = ball.currPos;
	Vec2f P2 = otherBall.currPos;
	float M1 = ball.mass;
	float M2 = otherBall.mass;
	float Mass = M1 + M2;

	Vec2f newV1 = V1 - ((P1 - P2) * (((2.0f * M2) / Mass) * (dot(V1 - V2, P1 - P2) / lengthSquared(P1 - P2))));
	Vec2f newV2 = V2 - ((P2 - P1) * (((2.0f * M1) / Mass) * (dot(V2 - V1, P2 - P1) / lengthSquared(P2 - P1))));
	newV1 = newV1 * bounceDampening;
	newV2 = newV2 * bounceDampening;
	
	setVelocity(ball, newV1);
	setVelocity(otherBall, newV2);

	float overlap = radiuses - distance;
	normalize(difference);
	Vec2f offset = difference * overlap;

	ball.currPos = ball.currPos + (offset * (M2 / Mass) * 1.1f);
	ball.prevPos = ball.prevPos + (offset * (M2 / Mass) * 1.1f);
	otherBall.currPos = otherBall.currPos - (offset * (M1 / Mass) * 1.1f);
	otherBall.prevPos = otherBall.prevPos - (offset * (M1 / Mass) * 1.1f);
}

void Simulation::collisions()
{
	for (int i = 0; i < numBalls; i++)
	{
		Ball& ball = balls[i];

		if (doSpatialPartitioning)
		{
			CellID cellId = getCellIdFromPosition(ball.currPos);
			for (int offsetx = -1; offsetx < 2; offsetx++)
			{
				for (int offsety = -1; offsety < 2; offsety++)
				{
					CellID neighbourCellId = cellId;
					neighbourCellId.cellX += offsetx;
					neighbourCellId.cellY += offsety;

					unsigned int neighbourCellKey = keyFromHash(hashCell(neighbourCellId));
					CellProperties cellProperty = startIndices[neighbourCellKey];

					for (int j = 0; j < cellProperty.size; j++)
					{
						BallCellKeyPair neighbourBallCellPair = ballKeyPairs[cellProperty.startIndex + j];
						int neighbourBallIndex = neighbourBallCellPair.ballIndex;
						if (i == neighbourBallIndex) continue;
						Ball& otherBall = balls[neighbourBallIndex];
						resolveCollision(ball, otherBall);
					}
				}
			}
		}
		else
		{
			for (int j = 0; j < numBalls; j++)
			{
				if (i == j) continue;
				Ball& otherBall = balls[j];
				resolveCollision(ball, otherBall);
			}
		}
	}
}

void Simulation::update()
{
	simulationProfiler.start();
	for (int i = 0; i < stepsPerFrame; i++)
	{
		partitionProfiler.start(); {
			if (doSpatialPartitioning)
				buildSpatialPartition();
		} partitionProfiler.stop();
		physicsProfiler.start(); {
			stepProfiler.start(); {
				step();
			} stepProfiler.stop();
			wallProfiler.start(); {
				walls();
			} wallProfiler.stop();
			collisionProfiler.start(); {
				collisions();
			} collisionProfiler.stop();
		} physicsProfiler.stop();
	}
	simulationProfiler.stop();
}

