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
	Simulation* simulation = nullptr;
	
	unsigned int circleShaderProgram;
	unsigned int circleVAO;
	unsigned int circleVBO;

	unsigned int lineShaderProgram;
	unsigned int lineVAO;
	unsigned int lineVBO;

	Profiler openglProfiler;
	Profiler inputProfiler;
	Profiler lineProfiler;
	Profiler circleProfile;
	Profiler guiProfiler;

	void update()
	{
		openglProfiler.start(); {
			inputProfiler.start(); {
				processInput();
			} inputProfiler.stop();
			clearScreen();
			lineProfiler.start(); {
				renderLines();
			} lineProfiler.stop();
			circleProfile.start(); {
				renderCircles();
			} circleProfile.stop();
			guiProfiler.start(); {
				renderGUI();
			} guiProfiler.stop();
			swapBuffers();
		} openglProfiler.stop();
	}

	// Utilities
	OpenGL(int initWidth, int initHeight, std::string title);
	~OpenGL();
	bool isAlive();
	void disableVSYNC();
	void enableVSYNC();
	void clearScreen();
	void swapBuffers();
	std::string loadSourceFile(std::string filePath);

	// Circle Rendering
	void initCircleRendering();
	void renderCircles();
	
	// Line Rendering
	void initLineRendering();
	void renderLines();

	// GUI
	void initImGui() const;
	void renderGUI();

	// User Input
	Vec2f getMousePosition();
	void processInput();
};

