
#include "opengl.h"

inline void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

OpenGL::OpenGL(int initWidth, int initHeight, std::string title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(initWidth, initHeight, title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, initWidth, initHeight);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	initCircleRendering();
	initLineRendering();
	initImGui();
}

OpenGL::~OpenGL()
{
	glDeleteVertexArrays(1, &circleVAO);
	glDeleteBuffers(1, &circleVBO);
	glDeleteProgram(circleShaderProgram);
	glfwTerminate();
}

bool OpenGL::isAlive()
{
	return !glfwWindowShouldClose(window);
}

void OpenGL::disableVSYNC()
{
	glfwSwapInterval(0);
}

void OpenGL::enableVSYNC()
{
	glfwSwapInterval(1);
}

void OpenGL::clearScreen(Simulation& simulation)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	int width, height; glfwGetFramebufferSize(window, &width, &height);
	simulation.screenWidth = width;
	simulation.screenHeight = height;
	simulation.max_u = (float)width / (float)height;
}

void OpenGL::swapBuffers()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

