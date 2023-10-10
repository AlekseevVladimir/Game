#pragma once

#include <string>
#include <memory>
#include "engine/core/component.h"
#include "engine/render/core/shaders_manager.h"

class GBufferShaderComponent : public Component
{
public:
	GBufferShaderComponent(std::string goID, std::shared_ptr<Shader> shader)
		: Component(goID), _shaderPtr(shader)
	{}

	std::shared_ptr<Shader> _shaderPtr;
};
