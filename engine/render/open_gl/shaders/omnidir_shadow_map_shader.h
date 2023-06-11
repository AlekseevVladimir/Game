#pragma once
#include "engine/render/open_gl/shaders/open_gl_shader.h"
#include "engine/transform/position_component.h"
#include "engine/render/core/shadows/omnidir_shadow_map_component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

class OmnidirShadowMapShader : public OpenGLShader
{
public:
	OmnidirShadowMapShader(std::string alias) : OpenGLShader(alias)
	{

	}

	virtual void setMatrices(GameObject* viewPointPtr) override
	{
		OmnidirShadowMapComponent* shadowMap = viewPointPtr->
			getComponent<OmnidirShadowMapComponent>();
		glm::vec3 lightPos = viewPointPtr->getComponent<PositionComponent>()->getPos().getGlm();
		std::vector<glm::mat4> shadowTransforms = 
		{
			shadowMap->_projectionMatrix * glm::lookAt(
				lightPos, lightPos + glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)),
			shadowMap->_projectionMatrix * glm::lookAt(
				lightPos, lightPos + glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)),
			shadowMap->_projectionMatrix * glm::lookAt(
				lightPos, lightPos + glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)),
			shadowMap->_projectionMatrix * glm::lookAt(
				lightPos, lightPos + glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, -1.f)),
			shadowMap->_projectionMatrix * glm::lookAt(
				lightPos, lightPos + glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, -1.f, 0.f)),
			shadowMap->_projectionMatrix * glm::lookAt(
				lightPos, lightPos + glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, -1.f, 0.f))
		};
		setFloat3("lightPos", &viewPointPtr->getComponent<PositionComponent>()->getPos()[0]);
		setFloat1("farPlane", shadowMap->_farPlane);
		std::string baseStr = "shadowMatrices[";
		for (int i = 0; i < 6; ++i)
		{
			setMatrix4Float(
				(baseStr + std::to_string(i) + "]").c_str(), GL_FALSE,
				glm::value_ptr(shadowTransforms[i])
			);
		}
	}

	virtual void configure() override
	{

	}
};