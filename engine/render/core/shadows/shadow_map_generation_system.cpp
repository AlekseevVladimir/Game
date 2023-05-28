#include "shadow_map_generation_system.h"
#include "engine/core/window_ctrl.h"
#include "engine/core/systems_holder.h"
#include "engine/render/core/render_system.h"
#include "engine/render/core/shadows/shadow_map_component.h"
#include "engine/render/core/shadows/shadow_map_shader_component.h"
#include "engine/render/core/shadows/omnidir_shadow_map_component.h"
#include "engine/render/core/shadows/omnidir_shadow_map_shader_component.h"
#include "glad/glad.h"


template<typename TShaderComponent>
void configureFramebuffer(
	GameObject* lightObjectPtr, unsigned int FBOID, 
	GLenum textureTargetType, unsigned int shadowMapID);

ShadowMapGenerationSystem::ShadowMapGenerationSystem()
{
	glGenFramebuffers(1, &m_depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glGenFramebuffers(1, &m_depthCubeMapFBO);
}

void ShadowMapGenerationSystem::process(float delta)
{
	
	for (GameObject* goPtr :
		GameObjectHolder::getInstance().getObjectsWithComponent<ShadowMapComponent>())
	{
		ShadowMapComponent* shadowMap = goPtr->getComponent<ShadowMapComponent>();
		glViewport(0, 0, shadowMap->_shadowWidth, shadowMap->_shadowHeight);
		unsigned int shadowMapID = shadowMap->m_shadowMapID;
		configureFramebuffer<ShadowMapShaderComponent>(goPtr, m_depthMapFBO, GL_TEXTURE_2D, shadowMapID);
		/*
		unsigned int shadowMapID = 
			goPtr->getComponent<ShadowMapComponent>()->m_shadowMapID;
		goPtr->getComponent<ShadowMapComponent>();
			//	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);


		unsigned int test = glCheckFramebufferStatus(m_depthMapFBO);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			throw(std::exception());
		}
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, shadowMapID);
		glCullFace(GL_FRONT);
		SystemsHolder::getInstance().getSystem<RenderSystem>()->
			render<ShadowMapShaderComponent>(goPtr);
		glCullFace(GL_BACK);
		*/
	}
	for (GameObject* goPtr :
		GameObjectHolder::getInstance().getObjectsWithComponent<OmnidirShadowMapComponent>())
	{
		unsigned int shadowMapID = goPtr->getComponent<OmnidirShadowMapComponent>()->m_shadowMapID;
		configureFramebuffer<OmnidirShadowMapShaderComponent>(goPtr, m_depthMapFBO, GL_TEXTURE_CUBE_MAP, shadowMapID);
	}
		
	
	glActiveTexture(GL_TEXTURE0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	WindowCtrl::getInstance().restoreWindow();
}

template<typename TShaderComponent>
void configureFramebuffer(
	GameObject* lightObjectPtr, unsigned int FBOID, 
	GLenum textureTargetType, unsigned int shadowMapID) 
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
		glClear(GL_DEPTH_BUFFER_BIT);
	//	unsigned int shadowMapID = 
	//		lightObjectPtr->getComponent<ShadowMapComponent>()->m_shadowMapID;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
			textureTargetType, shadowMapID, 0);
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);


		unsigned int test = glCheckFramebufferStatus(FBOID);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			throw(std::exception());
		}
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, shadowMapID);
		glCullFace(GL_FRONT);
		SystemsHolder::getInstance().getSystem<RenderSystem>()->
			render<TShaderComponent>(lightObjectPtr);
		glCullFace(GL_BACK);
}
