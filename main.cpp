
#include "opengl.h"
#include "simulation.h"
#include "profiler.h"


int main()
{
	OpenGL OpenGL(1920, 1080, "Verlet Integration");
	Simulation simulation;
	Profiler renderingProfiler;
	double renderingTime = 0.0;

	while (OpenGL.isAlive())
	{
		renderingProfiler.start(); {
			buildAndShipVertices(simulation);
			OpenGL.render(simulation.numBalls);
			OpenGL.renderGUI(renderingTime);
			OpenGL.swapBuffers();
		} renderingProfiler.stop();
		renderingTime = renderingProfiler.time();
	}

	return 0;
}

