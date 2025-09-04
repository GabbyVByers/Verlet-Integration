#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "random.h"
#include "profiler.h"
#include <iostream>

#include <thrust/execution_policy.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/sort.h>
#include <thrust/copy.h>

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
	int numBalls = 16384;
	Ball* balls = nullptr;

	float bounceDampening = 0.99f;
	int stepsPerFrame = 8;
	float deltaTime = 0.001f;
	float sub_dt = 0.0f;

	float max_u = 0.0f;
	int screenWidth = -1;
	int screenHeight = -1;

	bool doSpatialPartitioning = true;
	float gridWidth = 0.015f;
	int numUniqueCellKeys = 65535;
	BallCellKeyPair* ballKeyPairs = nullptr;
	CellProperties* startIndices = nullptr;
	thrust::device_vector<unsigned int> device_keys;
	thrust::host_vector<unsigned int> host_keys;

	Profiler profiler;
	double time_resizeHostKeys = 0.0;
	double time_resizeDeviceKeys = 0.0;
	double time_constructingUnorderedBallKeyPairs = 0.0;
	double time_memcpyHostToDevice = 0.0f;
	double time_GPU_SORT = 0.0f;
	double time_memcpyDeviceToHost = 0.0f;
	double time_unpackingSortedBallKeyPairs = 0.0;
	int numMembersOfHost = 0;
	int numMembersOfDevice = 0;

	Profiler simulationProfiler;
	Profiler partitionProfiler;
	Profiler ballKeysProfiler;
	Profiler startIndicesProfiler;
	Profiler physicsProfiler;
	Profiler stepProfiler;
	Profiler wallProfiler;
	Profiler collisionProfiler;

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
	void resolveCollision(int i, int j);
	void update();
};

