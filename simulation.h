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
	Color3f color;
	float radius = 0.0f;
	float mass = 0.0f;
};

struct cellID
{
	int cellX = 0;
	int cellY = 0;
};

struct ballCellKeyPair
{
	int ballIndex = -1;
	unsigned int cellKey = 0;
};

class Simulation
{
public:

	int numBalls = 100;
	Ball* balls = nullptr;

	float bounceDampening = 0.99f;
	int stepsPerFrame = 8;
	float deltaTime = 0.001f;
	float sub_dt = 0.0f;

	float max_u = 0.0f;
	float gridWidth = 0.25f;
	int screenWidth = -1;
	int screenHeight = -1;

	Simulation();
	~Simulation();

	cellID getCellIdFromPosition(Simulation& simulation, Vec2f position);
	unsigned int hashCell(cellID cellId);
	unsigned int keyFromHash(unsigned int hash, int numCells);
	void buildSpatialPartition(Simulation& simulation);

	Vec2f getVelocity(Ball& ball);
	void setVelocity(Ball& ball, Vec2f& velocity);
	void step();
	void walls();
	void collisions();
	void update();
};

