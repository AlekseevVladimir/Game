#pragma once

#include "engine/render/core/shadows/shadow_map_component.h"
#include "glad/glad.h"

class DirectionalShadowMapComponent : public ShadowMapComponent
{
public:
	DirectionalShadowMapComponent(std::string goID);

	void _bindToCurrentFramebuffer() override;
};
