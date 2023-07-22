#include "renderer.h"
#include "engine/render/open_gl/shaders/open_gl_shader.h"
#include "engine/render/open_gl/model/model_component.h"
#include "engine/render/core/shader_component.h"
#include "engine/render/core/shadows/shadow_map_shader_component.h"
#include "glad/glad.h"

OpenGL::Renderer::Renderer() : IRenderer(), _cullingType(GL_INVALID_ENUM)
{
	glEnable(GL_CULL_FACE);
}

void OpenGL::Renderer::render(GameObject* goPtr, GameObject* viewPointPtr, Shader* shaderPtr)
{
	OpenGLShader* OGLShaderPtr = dynamic_cast<OpenGLShader*>(shaderPtr);
	OGLShaderPtr->use();
	OGLShaderPtr->configure();
	OGLShaderPtr->setMatrices(viewPointPtr);
	Model* modelPtr = goPtr->getComponent<ModelComponent>()->model.get();
	OGLShaderPtr->setModelDataAndDraw(modelPtr, goPtr, viewPointPtr);
}

void OpenGL::Renderer::_setCullingType(unsigned int cullingType)
{
	if (cullingType != _cullingType)
	{
		glCullFace(cullingType);
		_cullingType = cullingType;
	}
}

