#pragma once
#define GLFW_INCLUDE_NONE
#include <iostream>
//#include <glad/glad.h>
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


class Shader 
{
public:
	virtual void use() = 0;
	
	virtual void configure() {};

	virtual ~Shader() = default;
};

class ShadersManager 
{
public:

	static ShadersManager& getInstance() 
	{
		static ShadersManager inst;
		return inst;
	}

	template<typename ShaderT>
	std::shared_ptr<ShaderT> createProgram(const std::string& alias) 
	{
		auto found = m_shaders.find(alias);
		if (found != m_shaders.end() && !found->second.expired()) 
		{
			return std::dynamic_pointer_cast<ShaderT>(found->second.lock());
		}
		std::shared_ptr<ShaderT> shaderPtr = std::make_shared<ShaderT>(alias);
		m_shaders[alias] = std::dynamic_pointer_cast<Shader>(shaderPtr);
		return shaderPtr;
	}

private:
	std::map<std::string, std::weak_ptr<Shader>> m_shaders;
};

