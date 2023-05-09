#include "render_system.h"
#include "engine/render/core/shader_component.h"
#include "engine/render/core/view_point_component.h"
#include "glad/glad.h"
#include "engine/render/open_gl/shaders/open_gl_shader.h"
#include "engine/render/open_gl/model/model_component.h"
#include "engine/render/open_gl/model/model.h"
#include "engine/render/core/shadows/shadow_map_shader_component.h"


void RenderSystem::process(float delta)
{

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GameObject* viewPointPtr = GameObjectHolder::getInstance().
		getObjectsWithComponent<ViewPointComponent>()[0];
	render<ShaderComponent>(viewPointPtr);

}


template<typename TShaderComponent>
void RenderSystem::render(GameObject* viewPointPtr)
{
	for (
		GameObject* goPtr :
		GameObjectHolder::getInstance().getObjectsWithComponent<TShaderComponent>())
	{
		std::shared_ptr<OpenGLShader> shaderPtr = 
			std::dynamic_pointer_cast<OpenGLShader>(
				goPtr->getComponent<TShaderComponent>()->m_shaderPtr);
		shaderPtr->use();
		shaderPtr->configure();	
		shaderPtr->setMatrices(viewPointPtr);
		ModelBase* modelPtr = goPtr->getComponent<ModelComponent>()->model.get();
		if (dynamic_cast<Model<ElementsMesh>*>(modelPtr))
		{
			shaderPtr->setModelDataAndDraw<Model<ElementsMesh>>(modelPtr, goPtr, viewPointPtr);
		}
		else
		{
			shaderPtr->setModelDataAndDraw<Model<Mesh>>(modelPtr, goPtr, viewPointPtr);
		}

	}
}

template void RenderSystem::render<ShadowMapShaderComponent>(GameObject*);
