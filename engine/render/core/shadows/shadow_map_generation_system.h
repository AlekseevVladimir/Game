#pragma once

#include "engine/core/system.h"

class ShadowMapFramebuffer
{
public:
	ShadowMapFramebuffer();

	virtual ~ShadowMapFramebuffer();

	virtual void _bindTextureAsTarget(unsigned int textureID) = 0;
	
	unsigned int _FBOID;
};

class ShadowMapGenerationSystem : public System
{
public:
	ShadowMapGenerationSystem();
	void process(float delta) override;

private:
	unsigned int m_depthMapFBO = 0;
	unsigned int m_depthCubeMapFBO = 0;
	unsigned int test = 0;
};