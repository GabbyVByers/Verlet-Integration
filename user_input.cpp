
#include "opengl.h"

Vec2f OpenGL::getMousePosition(Simulation& simulation)
{
	double dbl_mouseX;
	double dbl_mouseY;
	glfwGetCursorPos(window, &dbl_mouseX, &dbl_mouseY);
	Vec2f position;
	position.x = ((2.0f * simulation.max_u) * ((float)dbl_mouseX / simulation.screenWidth)) - simulation.max_u;
	position.y = ((((float)dbl_mouseY * -2.0f) / simulation.screenHeight) + 1.0f);
	return position;
}

void OpenGL::processInput(Simulation& simulation)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

