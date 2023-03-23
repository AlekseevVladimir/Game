#pragma once

#include <memory>
#define GLFW_INCLUDE_NONE
#include <engine/core/component.h>
#include "shader_base.h"

class ShaderComponent : public Component {
public:
	ShaderComponent(std::string goID, std::shared_ptr<ShaderBase> shaderPtr) : Component(goID), m_shaderPtr(shaderPtr) {}

private:
	std::shared_ptr<ShaderBase> m_shaderPtr;
};