#include "render_system.h"
#include "engine/render/core/g_buffer_shader_component.h"
#include "engine/render/core/lighting_shader_component.h"
#include "engine/render/open_gl/shaders/deferred_lighting_shader.h"
#include "engine/render/core/view_point_component.h"
#include "glad/glad.h"
#include "engine/render/open_gl/shaders/open_gl_shader.h"
#include "engine/render/open_gl/model/model_component.h"
#include "engine/render/open_gl/model/model.h"
#include "engine/render/core/shadows/shadow_map_shader_component.h"
#include "engine/render/core/shadows/omnidir_shadow_map_shader_component.h"
#include "engine/render/core/constants.h"
#include "engine/render/core/textures_ctrl.h"
#include <utility>



RenderSystem::RenderSystem() : System()
{
	glGenTextures(COLOR_BUFFERS_NUM, _colorBuffers);

	/*
	glGenFramebuffers(2, _bloomGenerationBuffers);
	glGenTextures(2, _bloomGenerationTextures);
	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _bloomGenerationBuffers[i]);
		TexturesCtrl::getInstance().bindTexture(_bloomGenerationTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RES_WIDTH, RES_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		// TODO read about filters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// TODO read about wrap params again
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _bloomGenerationTextures[i], 0);
	}
	*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glBindTexture(GL_TEXTURE_2D, 0);
	_HDRShader = ShadersManager::getInstance().createProgram<OpenGLShader>("hdr");
	_HDRMesh = std::make_unique<Mesh>(rectVertices, std::vector<Mesh::Texture>());
	_bloomShader = ShadersManager::getInstance().createProgram<OpenGLShader>("bloom");
	_bloomMesh = std::make_unique<Mesh>(rectVertices, std::vector<Mesh::Texture>());
	_GShader = ShadersManager::getInstance().createProgram<DeferredLightingShader>(
		"deferred_lighting");
	_GMesh = std::make_unique<Mesh>(rectVertices, std::vector<Mesh::Texture>());

/*
	glGenFramebuffers(1, &_gBuffer);
	glGenTextures(G_BUFFER_ITEMS_NUM, _gBufferItems);
	
	for (unsigned int i = 0; i < G_BUFFER_ITEMS_NUM; i++)
	{
		TexturesCtrl::getInstance().bindTexture(_gBufferItems[i]);
		// add some precision altering here for different gbuffer textures
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RES_WIDTH, RES_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _gBufferItems[i], 0);
	}
*/
	_gBuffer.m_setupFramebuffer();
	_postprocessBuffer.m_setupFramebuffer();
	
	for (auto& blurBuffer : m_blurBuffers)
	{
		blurBuffer.m_setupFramebuffer();
	}
}

/*
void RenderSystem::process(float delta)
{
	_setCullingType(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer.m_id);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GameObject* viewPointPtr = GameObjectHolder::getInstance().
		getObjectsWithComponent<ViewPointComponent>()[0];
	render<ShaderComponent>(viewPointPtr);
	_setCullingType(GL_FRONT);
}
*/

void RenderSystem::process(float delta)
{
	_setCullingType(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer.m_id);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GameObject* viewPointPtr = GameObjectHolder::getInstance().
		getObjectsWithComponent<ViewPointComponent>()[0];
	render<GBufferShaderComponent>(viewPointPtr);
	
	glBindFramebuffer(GL_FRAMEBUFFER, _postprocessBuffer.m_id);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// made kinda stupid mistake as per-object render was relevant only for filling g buffer
	// there is no need to render every object again as we already have all the data in 4 textures 
	// and only need to calculate light for them
	// (probably need to store fragment depths too)

	//_setCullingType(GL_FRONT);
	glDisable(GL_CULL_FACE);
	
	_GShader->use();
	glActiveTexture(GL_TEXTURE0);

	TexturesCtrl& texCtrl = TexturesCtrl::getInstance();
	_GShader->setInt1("gPosition", texCtrl.bindTexture(_gBuffer.m_positions, GL_TEXTURE_2D));
	_GShader->setInt1("gNormal", texCtrl.bindTexture(_gBuffer.m_normals, GL_TEXTURE_2D));
	_GShader->setInt1("gAlbedo", texCtrl.bindTexture(_gBuffer.m_albedo, GL_TEXTURE_2D));
	_GShader->setInt1("gSpecShine", texCtrl.bindTexture(_gBuffer.m_specShine, GL_TEXTURE_2D));
	// uniforms are optimized away as they are not used
	_GShader->setInt1("test", texCtrl.bindTexture(_gBuffer.m_specShine, GL_TEXTURE_2D));
	_GShader->configure();
	_GShader->setMatrices(viewPointPtr);
	_GMesh->draw();

	bool firstIter = true;
	bool horizontal = true;
	int totalIterations = 10;
	_bloomShader->use();

	//TODO this is kinda sheeeit and needs rework
	for (int i = 0; i < totalIterations; i++)
	{
		BlurFramebuffer& blurFramebufferFrom = m_blurBuffers[!horizontal];
		BlurFramebuffer& blurFramebufferTo = m_blurBuffers[horizontal];
		glBindFramebuffer(GL_FRAMEBUFFER, blurFramebufferTo.m_id);
		_bloomShader->setInt1("horizontal", horizontal);
		_bloomShader->setInt1("image", 
			TexturesCtrl::getInstance().bindTexture(
				firstIter ? _postprocessBuffer.m_brightness : blurFramebufferFrom.m_buffer,
				GL_TEXTURE_2D));
		_bloomMesh->draw();
		horizontal = !horizontal;
		if (firstIter)
		{
			firstIter = false;
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_HDRShader->use();
	glActiveTexture(GL_TEXTURE0);
	_HDRShader->setInt1("hdrBuffer", TexturesCtrl::getInstance().bindTexture(
		_postprocessBuffer.m_hdr, GL_TEXTURE_2D));
	_HDRShader->setInt1("bloomBuffer", TexturesCtrl::getInstance().bindTexture(
		m_blurBuffers[!horizontal].m_buffer, GL_TEXTURE_2D));
	//_HDRShader->setInt1("bloomBuffer", TexturesCtrl::getInstance().bindTexture(_bloomGenerationTextures[!horizontal]));
	_HDRShader->setFloat1("exposure", 1.f);
	_HDRMesh->draw();
	//glBindTexture(GL_TEXTURE_2D, 0);
	
}



template<typename TShaderComponent>
void RenderSystem::render(GameObject* viewPointPtr, bool ignoreViewPoint)
{
	for (
		GameObject* goPtr :
		GameObjectHolder::getInstance().getObjectsWithComponent<TShaderComponent>())
	{
		if (ignoreViewPoint && goPtr->getAlias() == viewPointPtr->getAlias())
		{
			continue;
		}
		m_renderer->render(
			goPtr, viewPointPtr, goPtr->getComponent<TShaderComponent>()->_shaderPtr.get());
	}
	
}

void RenderSystem::setRenderer(std::unique_ptr<IRenderer>&& rendererPtr)
{
	m_renderer = std::move(rendererPtr);
}

template void RenderSystem::render<ShadowMapShaderComponent>(GameObject*, bool);
template void RenderSystem::render<OmnidirShadowMapShaderComponent>(GameObject*, bool);

