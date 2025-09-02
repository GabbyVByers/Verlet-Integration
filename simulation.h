#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vec2f.h"
#include "random.h"

struct Ball
{
	Vec2f currPos;
	Vec2f prevPos;
	Vec2f acceleration;
	float radius;
	float mass;
	Color3f color;
};

class Simulation
{
public:

	int numBalls = 0;
	Ball* balls = nullptr;

	float bounceDampening = 0.99f;
	int stepsPerFrame = 8;
	float deltaTime = 0.001f;
	float sub_dt = 0.0f;

	float max_u = 0.0f;
	float gridWidth = 0.25f;
	int screenWidth = -1;
	int screenHeight = -1;

	Simulation()
	{
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

	~Simulation()
	{
		delete[] balls;
	}
};

