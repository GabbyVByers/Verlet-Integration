#pragma once

struct Ball
{
	Vec2f currPos;
	Vec2f prevPos;
	Vec2f acceleration;
	float radius;
	float mass;
	Color3f color;
};

struct Simulation
{
	int numBalls = -1;
	Ball* balls = nullptr;
	Vertex* vertices = nullptr;

	Simulation()
	{
		numBalls = 1000000;
		balls = new Ball[numBalls];
		vertices = new Vertex[numBalls];
		for (int i = 0; i < numBalls; i++)
		{
			Ball ball;
			ball.currPos = randomVec2f(-1.0f, 1.0f);
			ball.color = randomColor3f();
			ball.radius = 0.01f;
			balls[i] = ball;
		}
	}

	~Simulation()
	{
		delete[] balls;
		delete[] vertices;
	}
};

inline void buildAndShipVertices(Simulation& simulation)
{
	for (int i = 0; i < simulation.numBalls; i++)
	{
		Ball& ball = simulation.balls[i];
		Vertex vert = { ball.currPos.x, ball.currPos.y, ball.color.r, ball.color.g, ball.color.b, ball.radius };
		simulation.vertices[i] = vert;
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * simulation.numBalls, simulation.vertices, GL_STATIC_DRAW);
}

