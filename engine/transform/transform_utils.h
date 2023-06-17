#pragma once

#include <glm/glm.hpp>
#include <engine/engine_math/vector3.h>
#include <engine/transform/position_component.h>
#include <engine/transform/rotation_component.h>
#include "engine/transform/scale_component.h"
#include <glm/gtc/matrix_transform.hpp>

namespace TransformUtils
{
	glm::mat4 getTransform(
		PositionComponent* position,
		RotationComponent* rotation,
		ScaleComponent* scale = nullptr)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 pos = position->getPos().getGlm();//getWorldPos();
		model = glm::translate(model, pos);
		if (rotation->getYaw() > 0.0f) 
		{
			model = glm::rotate(model, glm::radians(rotation->getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (rotation->getRoll() > 0.0f) 
		{
			model = glm::rotate(model, glm::radians(rotation->getRoll()), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		if (rotation->getPitch() > 0.0f) 
		{
			model = glm::rotate(model, glm::radians(rotation->getPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		//glm::vec2 test = { m_scale.x, m_scale.y };
		glm::vec3 scaleVector = scale ? scale->getScale().getGlm() : glm::vec3(1.f, 1.f, 1.f);
		model = glm::scale(model, scaleVector);
		return model;
	}
}
