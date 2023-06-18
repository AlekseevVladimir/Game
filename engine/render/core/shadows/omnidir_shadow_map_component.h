#pragma once
#include "engine/render/core/shadows/shadow_map_component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


class OmnidirShadowMapComponent : public ShadowMapComponent
{
public:
	OmnidirShadowMapComponent(std::string goID);

	virtual ~OmnidirShadowMapComponent();

	void _bindToCurrentFramebuffer();

	float _farPlane = 25.f;

	glm::mat4 _projectionMatrix = glm::perspective(
		glm::radians(90.f), static_cast<float>(_shadowMapWidth) / _shadowMapHeight,
		1.f, _farPlane);
};
