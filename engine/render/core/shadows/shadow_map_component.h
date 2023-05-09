#pragma once

#include "engine/core/component.h"
#include "glad/glad.h"

class ShadowMapComponent : public Component
{
public:
	ShadowMapComponent(std::string goID);

	unsigned int m_shadowMapID;
};
