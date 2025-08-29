#pragma once

#include <string>
#include <fstream>
#include <sstream>

inline std::string loadSourceFile(std::string filePath)
{
	std::ifstream shaderFile(filePath);
	std::stringstream buffer;
	buffer << shaderFile.rdbuf();
	return buffer.str();
}

