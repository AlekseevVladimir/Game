#include <iostream>
#include "render_utils.h"
#include <engine/transform/position_component.h>
#include <engine/transform/rotation_component.h>
#include <glm/ext.hpp>
#include <glfw3.h>


glm::mat4 getView(GameObject* goPtr) 
{
	const float radius = 10.0f;
	Vector3<float> position = goPtr->getComponent<PositionComponent>()->getPos();
	Vector3<float> direction = goPtr->getComponent<RotationComponent>()->getDirection();
	//std::cout << "DIRECTION " << position.x << " " << position.y << " " << position.z << std::endl;
	glm::mat4 view = glm::lookAt(position.getGlm(), position.getGlm() + direction.getGlm(), glm::vec3(0.0f, 1.0f, 0.0f));
	
	return view;
}

glm::mat4 getProjection() 
{
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	return projection;
}

