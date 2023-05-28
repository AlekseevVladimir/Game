#pragma once

#include "open_gl_shader.h"
#include "engine/render/core/light/light_direction_component.h"

class ShadowMapShader : public OpenGLShader
{
public:
	ShadowMapShader(std::string alias) : OpenGLShader(alias)
	{};

	void setMatrices(GameObject* viewPointPtr) override 
	{
		// TODO fix this
		//ShadowMapComponent* shadowMapCmp = goPtr->getComponent<ShadowMapComponent>();
		LightDirectionComponent* lightDirComponent = 
			viewPointPtr->getComponent<LightDirectionComponent>();
		glm::mat4 lightProjection = glm::ortho(
			-30.f, 30.f, -30.f, 30.f, 1.f, 30.f);
		glm::mat lightView = glm::lookAt(
			lightDirComponent->getFrom().getGlm(),
			lightDirComponent->getTo().getGlm(),
			glm::vec3(0.f, 1.f, 0.f)  // vector up
		);
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		setMatrix4Float("lightSpaceMatrix", GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	};

	void configure() override 
	{
		// TODO fix this
		//ShadowMapComponent* shadowMapCmp = goPtr->getComponent<ShadowMapComponent>();
		glm::mat4 lightProjection = glm::ortho(
			-30.f, 30.f, -30.f, 30.f, 1.f, 30.f);
		glm::mat lightView = glm::lookAt(
			glm::vec3(-10.f, 10.f, -10.f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f, 1.f, 0.f)
		);
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		setMatrix4Float("lightSpaceMatrix", GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	};

protected:
	void setTextureData(std::vector<Mesh::Texture>& textures) {}
};
