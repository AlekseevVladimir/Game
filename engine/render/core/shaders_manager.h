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

};

class ShadersCtrl {
public:

	static ShadersCtrl& getInstance() {
		static ShadersCtrl inst;
		return inst;
	}

	template<typename ShaderT>
	std::shared_ptr<ShaderT> createProgram(const std::string& alias) {
		auto found = m_shaders.find(alias);
		if (found != m_shaders.end() && !found->second.expired()) {
			return found->second.lock();
		}
		std::shared_ptr<ShaderT> shaderPtr = std::make_shared<ShaderT>(alias);
		m_shaders[alias] = shaderPtr
		return shaderPtr;
	}

private:
	std::map<std::string, std::weak_ptr<Shader>> m_shaders;

	std::string m_currentlyUsed = "";

};
