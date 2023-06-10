#pragma once

#include "open_gl_shader.h"
#include "engine/render/core/light/light_direction_component.h"

class ShadowMapShader : public OpenGLShader
{
public:
	ShadowMapShader(std::string alias) : OpenGLShader(alias)
	{
	};

	void setMatrices(GameObject* viewPointPtr) override 
	{
		LightDirectionComponent* lightDirComponent = 
			viewPointPtr->getComponent<LightDirectionComponent>();
		glm::mat lightView = glm::lookAt(
			lightDirComponent->getFrom().getGlm(),
			lightDirComponent->getTo().getGlm(),
			glm::vec3(0.f, 1.f, 0.f)  // vector up
		);
		glm::mat4 lightSpaceMatrix = lightDirComponent->_projectionMatrix * lightView;
		setMatrix4Float("lightSpaceMatrix", GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	};


protected:
	void setTextureData(std::vector<Mesh::Texture>& textures) {}
};
