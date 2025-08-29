#pragma once

#include <chrono>

class Profiler
{
public:

	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point stopTime;
	std::chrono::duration<double, std::milli> duration;

	Profiler() {}
	~Profiler() {}

	void start()
	{
		startTime = std::chrono::high_resolution_clock::now();
	}

	void stop()
	{
		stopTime = std::chrono::high_resolution_clock::now();
		duration = stopTime - startTime;
	}

	double time() const
	{
		return duration.count();
	}
};
