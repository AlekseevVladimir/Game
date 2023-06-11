#include "render_system.h"
#include "engine/render/core/shader_component.h"
#include "engine/render/core/view_point_component.h"
#include "glad/glad.h"
#include "engine/render/open_gl/shaders/open_gl_shader.h"
#include "engine/render/open_gl/model/model_component.h"
#include "engine/render/open_gl/model/model.h"
#include "engine/render/core/shadows/shadow_map_shader_component.h"
#include "engine/render/core/shadows/omnidir_shadow_map_shader_component.h"


void RenderSystem::process(float delta)
{

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GameObject* viewPointPtr = GameObjectHolder::getInstance().
		getObjectsWithComponent<ViewPointComponent>()[0];
	render<ShaderComponent>(viewPointPtr);

}


template<typename TShaderComponent>
void RenderSystem::render(GameObject* viewPointPtr, bool ignoreViewPoint)
{
	//m_renderer->render(viewPointPtr);
	
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
		/*
		std::shared_ptr<OpenGLShader> shaderPtr = 
			std::dynamic_pointer_cast<OpenGLShader>(
				goPtr->getComponent<TShaderComponent>()->m_shaderPtr);
		shaderPtr->use();
		shaderPtr->configure();	
		shaderPtr->setMatrices(viewPointPtr);
		Model* modelPtr = goPtr->getComponent<ModelComponent>()->model.get();
		shaderPtr->setModelDataAndDraw(modelPtr, goPtr, viewPointPtr);
		*/
	}
	
}

void RenderSystem::setRenderer(std::unique_ptr<IRenderer>&& rendererPtr)
{
	m_renderer = std::move(rendererPtr);
}

template void RenderSystem::render<ShadowMapShaderComponent>(GameObject*, bool);
template void RenderSystem::render<OmnidirShadowMapShaderComponent>(GameObject*, bool);
