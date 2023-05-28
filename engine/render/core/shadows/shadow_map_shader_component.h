#pragma once

#include <memory>
#include "engine/core/component.h"
#include "engine/render/core/shaders_manager.h"

class ShadowMapShaderComponent : public Component
{
public:
	ShadowMapShaderComponent(std::string goID, std::shared_ptr<Shader> shaderPtr) : 
		Component(goID), _shaderPtr(shaderPtr) {};

	std::shared_ptr<Shader> _shaderPtr;
};