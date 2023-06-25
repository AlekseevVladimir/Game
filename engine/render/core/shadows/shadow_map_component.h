#pragma once
#include "engine/core/component.h"

class ShadowMapComponent : public Component
{
public:
	ShadowMapComponent(std::string alias, unsigned int shadowMapWidth = 4096, unsigned int shadowMapHeight = 4096);

	virtual ~ShadowMapComponent();

	virtual void _bindToCurrentFramebuffer() = 0;

	unsigned int _shadowMapID;
	unsigned int _shadowMapWidth;
	unsigned int _shadowMapHeight;
};