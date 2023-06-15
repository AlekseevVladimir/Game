#pragma once
#include "engine/core/component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


class OmnidirShadowMapComponent : public Component
{
public:
	OmnidirShadowMapComponent(std::string goID);

	unsigned int m_shadowMapID;
	unsigned int _shadowMapWidth = 2048;
	unsigned int _shadowMapHeight = 2048;
	float _farPlane = 25.f;
	glm::mat4 _projectionMatrix = glm::perspective(
		glm::radians(90.f), static_cast<float>(_shadowMapWidth) / _shadowMapHeight,
		1.f, _farPlane);
};