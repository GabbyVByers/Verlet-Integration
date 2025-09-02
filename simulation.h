#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vec2f.h"
#include "random.h"
#include <iostream>

struct Ball
{
	Vec2f currPos;
	Vec2f prevPos;
	Vec2f acceleration;
	Color3f color;
	float radius = 0.0f;
	float mass = 0.0f;
};

struct CellID
{
	int cellX = 0;
	int cellY = 0;
};

struct BallCellKeyPair
{
	int ballIndex = -1;
	unsigned int cellKey = 0;
};

struct CellProperties
{
	int startIndex = -1;
	int size = 0;
};

class Simulation
{
public:
	int numBalls = 1000;
	Ball* balls = nullptr;

	float bounceDampening = 0.99f;
	int stepsPerFrame = 8;
	float deltaTime = 0.001f;
	float sub_dt = 0.0f;

	float max_u = 0.0f;
	int screenWidth = -1;
	int screenHeight = -1;

	bool doSpatialPartitioning = false;
	float gridWidth = 0.055f;
	int numUniqueCellKeys = 5000;
	BallCellKeyPair* ballKeyPairs = nullptr;
	CellProperties* startIndices = nullptr;

	Simulation();
	~Simulation();

	CellID getCellIdFromPosition(Vec2f position);
	unsigned int hashCell(CellID cellId);
	unsigned int keyFromHash(unsigned int hash);
	void buildSpatialPartition();

	Vec2f getVelocity(Ball& ball);
	void setVelocity(Ball& ball, Vec2f& velocity);
	void step();
	void walls();
	void collisions();
	void resolveCollision(Ball& ball, Ball& other);
	void update();
};

