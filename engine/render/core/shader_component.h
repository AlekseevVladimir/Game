#pragma once

#include <memory>
#define GLFW_INCLUDE_NONE
#include <engine/core/component.h>
#include <engine/render/core/shaders_manager.h>

class ShaderComponent : public Component {
public:
	ShaderComponent(std::string goID, std::shared_ptr<Shader> shaderPtr) : 
		Component(goID), _shaderPtr(shaderPtr) {}

	std::shared_ptr<Shader> _shaderPtr;
};
