#pragma once

#include <memory>
#include "engine/core/component.h"

class Shader;

class OmnidirShadowMapShaderComponent : public Component
{
	OmnidirShadowMapShaderComponent(std::string goID, std::shared_ptr<Shader> shaderPtr) : Component(goID)
	{

	}

	std::shared_ptr<Shader> _shaderPtr;
};