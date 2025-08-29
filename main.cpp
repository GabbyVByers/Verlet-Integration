
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_loader.h"

#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

float randomFloat(float min, float max)
{
	return ((rand() / (float)RAND_MAX) * (max - min)) + min;
}

struct Vertex
{
	float x, y;
	float r, g, b;
	float radius;
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int initWidth = 1920;
	int initHeight = 1080;
	GLFWwindow* window = glfwCreateWindow(initWidth, initHeight, "We love OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, initWidth, initHeight);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	int numVertices = 50;
	Vertex* vertices = new Vertex[numVertices];

	for (int i = 0; i < numVertices; i++)
	{
		Vertex newVertex =
		{
			cos(i),
			sin(i),
			randomFloat(0.0f, 1.0),
			randomFloat(0.0f, 1.0),
			randomFloat(0.0f, 1.0),
			10.0f
		};
		vertices[i] = newVertex;
	}

	unsigned int VAO;
	unsigned int VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	std::string vert = loadSourceFile("shader.vert");
	std::string frag = loadSourceFile("shader.frag");
	const char* vertexShaderSource   = vert.c_str();
	const char* fragmentShaderSource = frag.c_str();

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glfwSwapInterval(0);

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		int width, height; glfwGetFramebufferSize(window, &width, &height); float aspectRaio = (float)width / (float)height;
		glUniform1f(glGetUniformLocation(shaderProgram, "uAspectRatio"), (float)aspectRaio);
		std::cout << aspectRaio << "\n";

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, numVertices);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

