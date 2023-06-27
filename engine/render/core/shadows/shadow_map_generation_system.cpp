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

void ShadowMapGenerationSystem::process(float delta)
{
	for (GameObject* goPtr :
		GameObjectHolder::getInstance().getObjectsWithComponent<DirectionalShadowMapComponent>())
	{
		configureFramebuffer<ShadowMapShaderComponent, DirectionalShadowMapComponent>(
			goPtr, m_depthMapFBO._FBOID, goPtr);
	}

	for (GameObject* goPtr :
		GameObjectHolder::getInstance().getObjectsWithComponent<OmnidirShadowMapComponent>())
	{
	
		configureFramebuffer<OmnidirShadowMapShaderComponent, OmnidirShadowMapComponent>(
			goPtr, m_depthCubeMapFBO._FBOID, goPtr);
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
	// TODO Do some abstraction here
	
	omnidirShadow->_bindToCurrentFramebuffer();


	unsigned int test = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		throw(std::exception());
	}
		
	//by default the culling is disabled TODO enable it. also there is some bright spot on troll's chest which should be in shadow
	//glCullFace(GL_FRONT);
	//glDisable(GL_CULL_FACE);
	SystemsHolder::getInstance().getSystem<RenderSystem>()->render<TShaderComponent>(lightObjectPtr);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_NONE);
}
