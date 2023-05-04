#include "render_system.h"
#include "engine/render/core/shader_component.h"
#include "engine/render/core/view_point_component.h"
#include "glad/glad.h"


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
		std::shared_ptr<Shader> shaderPtr = goPtr->getComponent<TShaderComponent>()->m_shaderPtr;
		shaderPtr->use();
		shaderPtr->configure();
		
		goPtr->getComponent<ModelComponent>()->model->setModelDataAndDraw(
			shaderPtr, goPtr, viewPointPtr);
	}
}
