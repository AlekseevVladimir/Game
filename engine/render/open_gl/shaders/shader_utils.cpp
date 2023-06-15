#include <glad/glad.h>
#include "shader_utils.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>


std::pair<bool, unsigned int> createShader(const std::string& shaderSrcFileName, int shaderType, unsigned int programID) {
	int success;
	unsigned int shaderID;
	char infoLog[512];
	const std::filesystem::path path{ "../../resources/shaders/" + shaderSrcFileName };
	std::ifstream shaderFstream(path);
	if (!shaderFstream.is_open())
	{
		return { false, 0 };
	}
	std::string shaderSrc;
	std::string buf;

	while (std::getline(shaderFstream, buf)) 
	{
		shaderSrc += buf;
		shaderSrc.push_back('\n');
	}
	shaderFstream.close();
	shaderID = glCreateShader(shaderType);
	const char* cstr = shaderSrc.c_str();
	glShaderSource(shaderID, 1, &cstr, NULL);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cerr << "Vertex shader compilation failed\n" << shaderSrcFileName <<
			std::endl << infoLog << std::endl;
		return { false, 0 };
	}


	glAttachShader(programID, shaderID);
	return { true, shaderID };

}