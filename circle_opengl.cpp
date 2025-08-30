
#include "opengl.h"

void OpenGL::initCircleRendering()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	std::string vert = loadSourceFile("shader.vert");
	std::string frag = loadSourceFile("shader.frag");
	const char* vertexShaderSource = vert.c_str();
	const char* fragmentShaderSource = frag.c_str();

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glEnable(GL_PROGRAM_POINT_SIZE);
}

void OpenGL::renderCircles(Simulation& simulation)
{
	Vertex* vertices = new Vertex[simulation.numBalls];
	for (int i = 0; i < simulation.numBalls; i++)
	{
		Ball& ball = simulation.balls[i];
		Vertex vert = { ball.currPos.x, ball.currPos.y, ball.color.r, ball.color.g, ball.color.b, ball.radius };
		vertices[i] = vert;
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * simulation.numBalls, vertices, GL_STATIC_DRAW);
	delete[] vertices;
	
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, simulation.numBalls);
}

