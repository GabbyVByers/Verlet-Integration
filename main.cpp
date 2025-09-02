
#include "opengl.h"
#include "profiler.h"

int main()
{
	OpenGL OpenGL(1920, 1080, "Verlet Integration");
	OpenGL.disableVSYNC();

	Simulation simulation;

	Profiler renderingProfiler;
	Profiler physicsProfiler;
	double renderingTime = 0.0;
	double physicsTime = 0.0;

	while (OpenGL.isAlive())
	{
		renderingProfiler.start(); {
			OpenGL.processInput(simulation);
			OpenGL.clearScreen(simulation);
			OpenGL.renderLines(simulation);
			OpenGL.renderCircles(simulation);
			OpenGL.renderGUI(renderingTime, physicsTime);
			OpenGL.swapBuffers();
		} renderingProfiler.stop();
		renderingTime = renderingProfiler.time();
		
		physicsProfiler.start(); {
			update(simulation);
		} physicsProfiler.stop();
		physicsTime = physicsProfiler.time();
	}

	return 0;
}

