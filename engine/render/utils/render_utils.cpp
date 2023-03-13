#include "render_utils.h"
#include <engine/transform/position_component.h>
#include <engine/transform/rotation_component.h>
#include <glm/ext.hpp>
#include <glfw3.h>


glm::mat4 getView(GameObject* goPtr) {
	/*
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
		*/
	const float radius = 10.0f;
	PositionComponent* position = goPtr->getComponent<PositionComponent>()->getPos();
	RotationComponent* direction = goPtr->getComponent<RotationComponent>()->getDirection();
	glm::mat4 view = glm::lookAt(position, position + direction.getGlm(), glm::vec3(0.0f, 1.0f, 0.0f));
	return view;
}

glm::mat4 getProjection() {
	glm::mat4 projection = glm::mat4();
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	return projection;
}
