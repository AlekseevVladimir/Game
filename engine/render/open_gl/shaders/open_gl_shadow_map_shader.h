#pragma once

#include "open_gl_shader.h"

class ShadowMapShader : public OpenGLShader
{
public:
	ShadowMapShader(std::string alias) : OpenGLShader(alias)
	{};

	void setMatrices(GameObject* viewPointPtr) override {};
	void configure() override 
	{
		//ShadowMapComponent* shadowMapCmp = goPtr->getComponent<ShadowMapComponent>();
		glm::mat4 lightProjection = glm::ortho(
			-10.f, 10.f, -10.f, 10.f, 1.f, 7.5f);
		glm::mat lightView = glm::lookAt(
			glm::vec3(-2.f, 4.f, -1.f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f, 1.f, 0.f)
		);
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		setMatrix4Float("lightSpaceMatrix", GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	};

protected:
	void setTextureData(std::vector<Mesh::Texture>& textures) {}
};
