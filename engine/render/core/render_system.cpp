#include "render_system.h"
#include "engine/render/core/shader_component.h"
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
	//glBindTexture(GL_TEXTURE_2D, _HDRTexture);
	

	unsigned int tmpDepthBuffer;
	glGenRenderbuffers(1, &tmpDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, tmpDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RES_WIDTH, RES_HEIGHT);

	glGenFramebuffers(1, &_postprocessFBOID);
	glBindFramebuffer(GL_FRAMEBUFFER, _postprocessFBOID);

	for (unsigned int i = 0; i < COLOR_BUFFERS_NUM; i++)
	{
		TexturesCtrl::getInstance().bindTexture(_colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RES_WIDTH, RES_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorBuffers[i], 0);
	}
	unsigned int attachmentTypes[COLOR_BUFFERS_NUM] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(COLOR_BUFFERS_NUM, attachmentTypes);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tmpDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glBindTexture(GL_TEXTURE_2D, 0);
	_HDRShader = ShadersManager::getInstance().createProgram<OpenGLShader>("hdr");
	_HDRMesh = std::make_unique<Mesh>(rectVertices, std::vector<Mesh::Texture>());
	_bloomShader = ShadersManager::getInstance().createProgram<OpenGLShader>("bloom");
	_bloomMesh = std::make_unique<Mesh>(rectVertices, std::vector<Mesh::Texture>());
}

void RenderSystem::process(float delta)
{
	_setCullingType(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, _postprocessFBOID);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GameObject* viewPointPtr = GameObjectHolder::getInstance().
		getObjectsWithComponent<ViewPointComponent>()[0];
	render<ShaderComponent>(viewPointPtr);
	
	_setCullingType(GL_FRONT);

	bool firstIter = true;
	bool horizontal = true;
	int totalIterations = 10;
	_bloomShader->use();

	for (int i = 0; i < totalIterations; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _bloomGenerationBuffers[horizontal]);
		_bloomShader->setInt1("horizontal", horizontal);
		_bloomShader->setInt1("image", 
			TexturesCtrl::getInstance().bindTexture(
				firstIter ? _colorBuffers[ColorBuffers::Brightness] : _bloomGenerationTextures[!horizontal]));
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
	//glBindTexture(GL_TEXTURE_2D, _HDRTexture);
	_HDRShader->setInt1("hdrBuffer", TexturesCtrl::getInstance().bindTexture(_colorBuffers[ColorBuffers::HDR]));
	_HDRShader->setInt1("bloomBuffer", TexturesCtrl::getInstance().bindTexture(_bloomGenerationTextures[!horizontal]));
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

