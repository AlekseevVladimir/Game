#include <iostream>
#include "shadow_map_generation_system.h"
#include "engine/core/window_ctrl.h"
#include "engine/core/systems_holder.h"
#include "engine/render/core/render_system.h"
#include "engine/render/core/shadows/directional_shadow_map_component.h"
#include "engine/render/core/shadows/shadow_map_shader_component.h"
#include "engine/render/core/shadows/omnidir_shadow_map_component.h"
#include "engine/render/core/shadows/omnidir_shadow_map_shader_component.h"
#include "glad/glad.h"


template<typename TShaderComponent, typename TShadowMapComponent>
void configureFramebuffer(
	GameObject* lightObjectPtr, unsigned int FBOID,
	GameObject* goPtr);

// TODO remove opengl from core
ShadowMapFramebuffer::ShadowMapFramebuffer()
{
	glGenFramebuffers(1, &_FBOID);
	glBindFramebuffer(GL_FRAMEBUFFER, _FBOID);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

ShadowMapFramebuffer::~ShadowMapFramebuffer()
{
	glDeleteFramebuffers(1, &_FBOID);
}

ShadowMapGenerationSystem::ShadowMapGenerationSystem()
{
	glGenFramebuffers(1, &m_depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &m_depthCubeMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthCubeMapFBO);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapGenerationSystem::process(float delta)
{
	for (GameObject* goPtr :
		GameObjectHolder::getInstance().getObjectsWithComponent<DirectionalShadowMapComponent>())
	{
		configureFramebuffer<ShadowMapShaderComponent, DirectionalShadowMapComponent>(
			goPtr, m_depthMapFBO, goPtr);
	}

	for (GameObject* goPtr :
		GameObjectHolder::getInstance().getObjectsWithComponent<OmnidirShadowMapComponent>())
	{
	
		configureFramebuffer<OmnidirShadowMapShaderComponent, OmnidirShadowMapComponent>(
			goPtr, m_depthCubeMapFBO, goPtr);
	}
		
	
	glActiveTexture(GL_TEXTURE0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	WindowCtrl::getInstance().restoreWindow();
}

template<typename TShaderComponent, typename TShadowMapComponent>
void configureFramebuffer(
	GameObject* lightObjectPtr, unsigned int FBOID, GameObject* goPtr) 
{
	TShadowMapComponent* omnidirShadow = goPtr->getComponent<TShadowMapComponent>();
	glViewport(0, 0, omnidirShadow->_shadowMapWidth, omnidirShadow->_shadowMapHeight);
	unsigned int shadowMapID = omnidirShadow->_shadowMapID;

	glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
	glClear(GL_DEPTH_BUFFER_BIT);
	// TODO fix messy if statement here
	/*
	if (FBOID == 2)
	{
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			shadowMapID, 0);
	}
	else 
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
			textureTargetType, shadowMapID, 0);
	}
	*/
	omnidirShadow->_bindToCurrentFramebuffer();


	unsigned int test = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		throw(std::exception());
	}
		
	glCullFace(GL_FRONT);
	SystemsHolder::getInstance().getSystem<RenderSystem>()->
		render<TShaderComponent>(lightObjectPtr);
	glCullFace(GL_BACK);
}
