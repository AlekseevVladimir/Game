#pragma once
#include <string>
#include <iostream>

#include <glfw3.h>
#include <glad/glad.h>

#include "utils.h"

class ShaderBase {
public:
	ShaderBase(std::string alias) {
		unsigned int programID = glCreateProgram();
		auto vertexShaderCreationRes = createShader(alias + ".vs", GL_VERTEX_SHADER, programID);
		if (!vertexShaderCreationRes.first) {
			return;
		}
		auto fragmentShaderCreationRes = createShader(alias + ".fs", GL_FRAGMENT_SHADER, programID);
		if (!fragmentShaderCreationRes.first) {
			return;
		}

		glLinkProgram(programID);
		int success;
		char infoLog[512];
		glGetProgramiv(programID, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cerr << "Shader program linking failed \n" << infoLog << std::endl;
			return;

		}
		glDeleteShader(vertexShaderCreationRes.second);
		glDeleteShader(fragmentShaderCreationRes.second);
		m_programID = programID;
	}

	virtual void use() {
		glUseProgram(m_programID);
	}

	void setInt1(const char* name, int value) {
		glUniform1i(glGetUniformLocation(m_programID, name), value);
	}

	void setFloat1(const char* name, float value) {
		glUniform1f(glGetUniformLocation(m_programID, name), value);
	}

	void setFloat3(const char* name, const float* values) {
		glUniform3fv(glGetUniformLocation(m_programID, name), 1, values);
	}

	void setFloat3(const char* name, float val1, float val2, float val3) {
		glUniform3f(
			glGetUniformLocation(m_programID, name),
			val1, val2, val3);
	}

	void setMatrix4Float(const char* name, int transpose, const float* values) {
		glUniformMatrix4fv(
			glGetUniformLocation(m_programID, name),
			1, transpose, values);
	}

private:
	unsigned int m_programID;
};