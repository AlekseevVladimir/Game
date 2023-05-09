#include "shadow_map_generation_system.h"
#include "engine/core/window_ctrl.h"
#include "engine/core/systems_holder.h"
#include "engine/render/core/render_system.h"
#include "engine/render/core/shadows/shadow_map_component.h"
#include "engine/render/core/shadows/shadow_map_shader_component.h"
#include "glad/glad.h"

namespace 
{
	int SHADOW_WIDTH = 1024;
	int SHADOW_HEIGHT = 1024;
}

ShadowMapGenerationSystem::ShadowMapGenerationSystem()
{
	glGenFramebuffers(1, &m_depthMapFBO);
}

void ShadowMapGenerationSystem::process(float delta)
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glReadBuffer(GL_NONE);
	for (GameObject* goPtr :
		GameObjectHolder::getInstance().getObjectsWithComponent<ShadowMapComponent>())
	{
		goPtr->getComponent<ShadowMapComponent>();
		unsigned int shadowMapID = goPtr->getComponent<ShadowMapComponent>()->m_shadowMapID;
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
	}
	glActiveTexture(GL_TEXTURE0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	WindowCtrl::getInstance().restoreWindow();
}

