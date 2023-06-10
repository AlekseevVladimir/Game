#pragma once

#include "engine/core/system.h"


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