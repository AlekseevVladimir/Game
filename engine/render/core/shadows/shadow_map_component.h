#pragma once
#include "engine/core/component.h"

class ShadowMapComponent : public Component
{
public:
	ShadowMapComponent(std::string alias, unsigned int shadowMapWidth = 2048, unsigned int shadowMapHeight = 2048);

	virtual ~ShadowMapComponent();

	virtual void _bindToCurrentFramebuffer() = 0;

	unsigned int _shadowMapID;
	unsigned int _shadowMapWidth;
	unsigned int _shadowMapHeight;
};