
#include "opengl.h"

struct CircleVertex
{
	float x, y;
	float r, g, b;
	float radius;
};

void OpenGL::initCircleRendering()
{
	glGenVertexArrays(1, &circleVAO);
	glGenBuffers(1, &circleVBO);

	glBindVertexArray(circleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, circleVBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	std::string vert = loadSourceFile("circle_shader.vert");
	std::string frag = loadSourceFile("circle_shader.frag");
	const char* vertShaderSource = vert.c_str();
	const char* fragShaderSource = frag.c_str();

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderSource, nullptr);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSource, nullptr);
	glCompileShader(fragmentShader);

	circleShaderProgram = glCreateProgram();
	glAttachShader(circleShaderProgram, vertexShader);
	glAttachShader(circleShaderProgram, fragmentShader);
	glLinkProgram(circleShaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glEnable(GL_PROGRAM_POINT_SIZE);
}

void OpenGL::renderCircles(Simulation& simulation)
{
	glBindVertexArray(circleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
	glUseProgram(circleShaderProgram);

	std::vector<CircleVertex> circleVertices;
	for (int i = 0; i < simulation.numBalls; i++)
	{
		Ball& ball = simulation.balls[i];
		circleVertices.push_back({ ball.currPos.x, ball.currPos.y, ball.color.r, ball.color.g, ball.color.b, ball.radius });
	}
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(CircleVertex) * simulation.numBalls, &circleVertices[0], GL_STATIC_DRAW);
	glUniform2f(glGetUniformLocation(circleShaderProgram, "uScreenDimensions"), (float)simulation.screenWidth, (float)simulation.screenHeight);
	glDrawArrays(GL_POINTS, 0, (GLsizei)simulation.numBalls);
}

