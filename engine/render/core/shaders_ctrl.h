#pragma once
#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <set>
#include <map>
#include <memory>
#include <utility>
#include <filesystem>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <engine/core/game_object.h>
#include <glfw3.h>

std::pair<bool, unsigned int> createShader(const std::string& shaderSrcFileName, int shaderType, unsigned int programID);

class Shader {
public:
	struct Props {
		bool usesLightData = true;
		bool usesMaterialData = true;
	};

	Shader(const std::string& alias, Props props) : m_props(props) {
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

	const Props getProps() {
		return m_props;
	}

	virtual void setup(std::weak_ptr<GameObject> go) {};

	virtual void draw(std::weak_ptr<GameObject> go) {};

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


protected:
	unsigned int m_programID = 0;
	const Props m_props;
};

void setLightData(std::shared_ptr<Shader> shader);
void drawObject(std::shared_ptr<Shader> shader, GameObject* go, GameObject* viewPoint);

class ShadersCtrl {
public:

	static ShadersCtrl& getInstance() {
		static ShadersCtrl inst;
		return inst;
	}

	std::weak_ptr<Shader> createProgram(const std::string& alias, Shader::Props shaderProps = {}) {
		auto found = m_shaders.find(alias);
		if (found != m_shaders.end()) {
			return found->second;
		}
		m_shaders[alias] = std::make_shared<Shader>(alias, shaderProps);
		return m_shaders[alias];
	}

	void draw(const std::string& alias, std::weak_ptr<GameObject> go) {
		m_shaders.at(alias)->draw(go);
	}

	std::shared_ptr<Shader> useProgram(const std::string& alias) {
		
		std::shared_ptr<Shader> shader = m_shaders.at(alias);
		shader->use();

		m_currentlyUsed = alias;
		return shader;
	}


	void setupShader(const std::string& alias, std::weak_ptr<GameObject> go) {
		m_shaders.at(alias)->setup(go);
	}


	unsigned int getProgramID(const std::string& alias) const {
		return m_programs.at(alias);
	}

private:
	std::map<std::string, unsigned int> m_programs;
	std::map<std::string, std::shared_ptr<Shader>> m_shaders;

	std::string m_currentlyUsed = "";

};
