#pragma once

#include <memory>

#include <core/component.h>
#include "shader_base.h"

class ShaderComponent : public Component {
public:
	ShaderComponent(std::string goID, std::shared_ptr<ShaderBase> shaderPtr) : Component(goID), m_shaderPtr(shaderPtr) {}

private:
	std::shared_ptr<ShaderBase> m_shaderPtr;
};