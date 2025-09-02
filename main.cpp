
#include "opengl.h"
#include "profiler.h"

int main()
{
	Simulation simulation;
	OpenGL OpenGL(1920, 1080, "Verlet Integration");
	
	OpenGL.simulation = &simulation;
	OpenGL.disableVSYNC();

	while (OpenGL.isAlive())
	{
		OpenGL.update();
		simulation.update();
	}

	return 0;
}

