
#include "opengl.h"

struct LineVertex
{
	float x, y;
	float r, g, b;
};

void OpenGL::initLineRendering()
{
	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);

	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	std::string vert = loadSourceFile("line_shader.vert");
	std::string frag = loadSourceFile("line_shader.frag");
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

	lineShaderProgram = glCreateProgram();
	glAttachShader(lineShaderProgram, vertexShader);
	glAttachShader(lineShaderProgram, fragmentShader);
	glLinkProgram(lineShaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glEnable(GL_PROGRAM_POINT_SIZE);
}

void OpenGL::renderLines(Simulation& simulation)
{
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glUseProgram(lineShaderProgram);

	std::vector<LineVertex> lineVertices;
	float max_u = simulation.max_u;
	float gridWidth = simulation.gridWidth;

	lineVertices.push_back({ -max_u,  0.0f, 1.0f, 1.0f, 1.0f });
	lineVertices.push_back({  max_u,  0.0f, 1.0f, 1.0f, 1.0f });
	lineVertices.push_back({   0.0f, -1.0f, 1.0f, 1.0f, 1.0f });
	lineVertices.push_back({   0.0f,  1.0f, 1.0f, 1.0f, 1.0f });

	int columns = (max_u / gridWidth) + 1;
	for (int i = 0; i < columns; i++)
	{
		lineVertices.push_back({   gridWidth * (i + 1) - (gridWidth  * 0.5f),  -1.0f, 0.3f, 0.3f, 0.3f });
		lineVertices.push_back({   gridWidth * (i + 1) - (gridWidth  * 0.5f),   1.0f, 0.3f, 0.3f, 0.3f });
		lineVertices.push_back({ -(gridWidth * (i + 1) - (gridWidth  * 0.5f)), -1.0f, 0.3f, 0.3f, 0.3f });
		lineVertices.push_back({ -(gridWidth * (i + 1) - (gridWidth  * 0.5f)),  1.0f, 0.3f, 0.3f, 0.3f });
	}

	int rows = (1.0f / gridWidth);
	for (int i = 0; i < rows; i++)
	{
		lineVertices.push_back({ -max_u,   gridWidth * (i + 1) - (gridWidth * 0.5f),  0.3f, 0.3f, 0.3f });
		lineVertices.push_back({  max_u,   gridWidth * (i + 1) - (gridWidth * 0.5f),  0.3f, 0.3f, 0.3f });
		lineVertices.push_back({ -max_u, -(gridWidth * (i + 1) - (gridWidth * 0.5f)), 0.3f, 0.3f, 0.3f });
		lineVertices.push_back({  max_u, -(gridWidth * (i + 1) - (gridWidth * 0.5f)), 0.3f, 0.3f, 0.3f });
	}

	for (int i = 0; i < (int)max_u + 1; i++)
	{
		lineVertices.push_back({  (float)i, -0.04f, 1.0f, 1.0f, 1.0f });
		lineVertices.push_back({  (float)i,  0.04f, 1.0f, 1.0f, 1.0f });
		lineVertices.push_back({ -(float)i, -0.04f, 1.0f, 1.0f, 1.0f });
		lineVertices.push_back({ -(float)i,  0.04f, 1.0f, 1.0f, 1.0f });
	}

	lineVertices.push_back({ -0.04f, -0.999f, 1.0f, 1.0f, 1.0f });
	lineVertices.push_back({  0.04f, -0.999f, 1.0f, 1.0f, 1.0f });
	lineVertices.push_back({ -0.04f,  0.999f, 1.0f, 1.0f, 1.0f });
	lineVertices.push_back({  0.04f,  0.999f, 1.0f, 1.0f, 1.0f });

	glBufferData(GL_ARRAY_BUFFER, sizeof(LineVertex) * lineVertices.size(), &lineVertices[0], GL_STATIC_DRAW);
	glUniform2f(glGetUniformLocation(lineShaderProgram, "uScreenDimensions"), (float)simulation.screenWidth, (float)simulation.screenHeight);
	glDrawArrays(GL_LINES, 0, lineVertices.size());
}

