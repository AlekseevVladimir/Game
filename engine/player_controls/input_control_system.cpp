#include <glfw3.h>
#include <iostream>

#include "input_control_system.h"
#include <engine/transform/movement_request_component.h>
#include <engine/transform/rotation_request_component.h>
#include <engine/transform/position_component.h>
#include <engine/transform/rotation_component.h>
#include <engine/player_controls/player_controls_component.h>
#include <engine/core/game_object.h>
#include <engine/core/window_ctrl.h>
#include <engine/transform/movement_request_component.h>
#include <engine/player_controls/player_controls_component.h>

void InputControlSystem::processInput() {
	const float cameraSpeed = 0.1f; 
	GLFWwindow* window = WindowCtrl::getInstance().getWindow();
	Vector3<float> movementModifier;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		movementModifier.z++;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		movementModifier.z--;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		movementModifier.x--;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		movementModifier.x++;
	}
	if (!movementModifier.isZero()) {
		for (auto goPtr : GameObjectHolder::getInstance().getObjectsWithComponents<PlayerControlsComponent, PositionComponent>()) {
			Vector3<float> direction = goPtr->getComponent<RotationComponent>()->getDirection();
			Vector3<float> modifierBuf(movementModifier);
			movementModifier = direction.cross({ 0.0f, 1.0f, 0.0f }) * modifierBuf.x +
				direction * modifierBuf.z;
			goPtr->createComponent<MovementRequestComponent>(movementModifier);
			//std::cout << "test" << MovementRequestComponent::getComponentName() << std::endl;
		}
	}
}

void InputControlSystem::processMousePos() {
	double xpos, ypos;
	glfwGetCursorPos(WindowCtrl::getInstance().getWindow(), &xpos, &ypos);
	if (m_firstMouse)
	{
		m_lastX = xpos;
		m_lastY = ypos;
		m_firstMouse = false;
	}
	float xoffset = static_cast<float>(xpos) - m_lastX;
	float yoffset = m_lastY - static_cast<float>(ypos); 
	m_lastX = xpos;
	m_lastY = ypos;

	const float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	for (auto goPtr : GameObjectHolder::getInstance().getObjectsWithComponents<PlayerControlsComponent, RotationComponent>()) {
		auto transform = goPtr->getComponent<RotationComponent>();
		float yaw = transform->getYaw();
		float pitch = transform->getPitch();
		yaw += xoffset;
		pitch += yoffset;
		if (!xoffset && !yoffset) {
			continue;
		}
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		goPtr->createComponent<RotationRequestComponent>(xoffset, yoffset, 0);
	}
}
