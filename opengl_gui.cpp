
#include "opengl.h"

void OpenGL::initImGui() const
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 2.0f;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void OpenGL::renderGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	double simulationTime = simulation->simulationProfiler.time();
	double partitionTime = simulation->partitionProfiler.time();
	double ballKeysTime = simulation->ballKeysProfiler.time();
	double startIndicesTime = simulation->startIndicesProfiler.time();
	double physicsTime = simulation->physicsProfiler.time();
	double stepTime = simulation->stepProfiler.time();
	double wallTime = simulation->wallProfiler.time();
	double collisionTime = simulation->collisionProfiler.time();

	double openglTime = openglProfiler.time();
	double inputTime = inputProfiler.time();
	double lineTime = lineProfiler.time();
	double circleTime = circleProfile.time();
	double guiTime = guiProfiler.time();

	ImGui::Begin("Profiler");
	ImGui::Text("Simulation          (ms): %g", simulationTime);
	ImGui::Text("   Partition           (ms): %g", partitionTime * (double)simulation->stepsPerFrame);
	ImGui::Text("      Ball Keys           (ms): %g", ballKeysTime * (double)simulation->stepsPerFrame);
	ImGui::Text("      Start Indices       (ms): %g", startIndicesTime * (double)simulation->stepsPerFrame);
	ImGui::Text("   Physics             (ms): %g", physicsTime * (double)simulation->stepsPerFrame);
	ImGui::Text("      Step                (ms): %g", stepTime * (double)simulation->stepsPerFrame);
	ImGui::Text("      Walls               (ms): %g", wallTime * (double)simulation->stepsPerFrame);
	ImGui::Text("      Collisions          (ms): %g", collisionTime * (double)simulation->stepsPerFrame); 
	ImGui::Text("OpenGL              (ms): %g", openglTime);
	ImGui::Text("   Lines               (ms): %g", lineTime);
	ImGui::Text("   Circles             (ms): %g", circleTime);
	ImGui::Text("   Input               (ms): %g", inputTime);
	ImGui::Text("   ImGUI               (ms): %g", guiTime);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

 