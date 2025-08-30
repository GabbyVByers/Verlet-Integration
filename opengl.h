#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "shader_loader.h"
#include "physics.h"
#include <string>
#include <iostream>

struct Vertex
{
	float x, y;
	float r, g, b;
	float radius;
};

class OpenGL
{
public:

	GLFWwindow* window = nullptr;
	unsigned int shaderProgram;
	unsigned int VAO;
	unsigned int VBO;

	// Utilities
	OpenGL(int initWidth, int initHeight, std::string title);
	~OpenGL();
	bool isAlive();
	void disableVSYNC();
	void enableVSYNC();
	void startRender(Simulation& simulation);
	void swapBuffers();

	// Circles
	void initCircleRendering();
	void renderCircles(Simulation& simulation);

	// GUI
	void initImGui() const;
	void renderGUI(double renderTime, double physicsTime);

	// User Input
	void processInput();
};

