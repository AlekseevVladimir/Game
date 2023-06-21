#pragma once

#include "engine/core/system.h"

class ShadowMapFramebuffer
{
public:
	ShadowMapFramebuffer();

	virtual ~ShadowMapFramebuffer();

	unsigned int _FBOID;
};

class ShadowMapGenerationSystem : public System
{
public:
	void process(float delta) override;

private:
	ShadowMapFramebuffer m_depthMapFBO;
	ShadowMapFramebuffer m_depthCubeMapFBO;
	unsigned int test = 0;
};