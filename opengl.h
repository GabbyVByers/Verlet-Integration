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

inline void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

class OpenGL
{
public:

	GLFWwindow* window = nullptr;
	unsigned int shaderProgram;
	unsigned int VAO;
	unsigned int VBO;
	Vertex* vertices = nullptr;

	OpenGL(int initWidth, int initHeight, std::string title)
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

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.FontGlobalScale = 2.0f;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	~OpenGL()
	{
		glfwTerminate();
	}

	void disableVSYNC() { glfwSwapInterval(0); }
	void enableVSYNC() { glfwSwapInterval(1); }

	bool isAlive()
	{
		return !glfwWindowShouldClose(window);
	}

	void processInput()
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	void render(Simulation& simulation)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		int width, height; glfwGetFramebufferSize(window, &width, &height);
		simulation.screenWidth = width;
		simulation.screenHeight = height;
		glUniform2f(glGetUniformLocation(shaderProgram, "uScreenDimensions"), (float)width, (float)height);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, simulation.numBalls);
	}

	void renderGUI(double renderTime, double physicsTime)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Profiler");
		ImGui::Text("Rendering (ms): %g", renderTime);
		ImGui::Text("Physics   (ms): %g", physicsTime);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void swapBuffers()
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
};

