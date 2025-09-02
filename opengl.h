#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "simulation.h"

class OpenGL
{
public:

	GLFWwindow* window = nullptr;
	
	unsigned int circleShaderProgram;
	unsigned int circleVAO;
	unsigned int circleVBO;

	unsigned int lineShaderProgram;
	unsigned int lineVAO;
	unsigned int lineVBO;

	// Utilities
	OpenGL(int initWidth, int initHeight, std::string title);
	~OpenGL();
	bool isAlive();
	void disableVSYNC();
	void enableVSYNC();
	void clearScreen(Simulation& simulation);
	void swapBuffers();
	std::string loadSourceFile(std::string filePath);

	// Circle Rendering
	void initCircleRendering();
	void renderCircles(Simulation& simulation);
	
	// Line Rendering
	void initLineRendering();
	void renderLines(Simulation& simulation);

	// GUI
	void initImGui() const;
	void renderGUI(double renderTime, double physicsTime);

	// User Input
	Vec2f getMousePosition(Simulation& simulation);
	void processInput(Simulation& simulation);
};

