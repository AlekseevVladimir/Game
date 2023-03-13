#include "window_ctrl.h"
#include <engine/player_controls/player_controls_component.h>

#include <iostream>
#include <glad/glad.h>
#include <engine/render/core/shaders_ctrl.h>
#include <memory>
//#include <Render/TexturesCtrl.h>
//#include <algorithm>
#include <engine/utils/engine_constants.h>
#include <engine/core/game_object.h>
//#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include <glfw3.h>
#include <engine/core/systems_holder.h>
#include <engine/player_controls/input_control_system.h>

void WindowCtrl::init() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(RES_WIDTH, RES_HEIGHT, "First OpenGL app", NULL, NULL);
	if (m_window == NULL) {
		std::cerr << "Failed to create window" << std::endl;
		return;
	}
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	m_isValid = true;
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return;
	}
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetKeyCallback(m_window, keyCallback);
	glfwSetCursorPosCallback(m_window, mouseCallback);
	glfwSetWindowUserPointer(m_window, this);
}


void WindowCtrl::draw() {
	if (glfwWindowShouldClose(m_window) || !m_isValid) {
		m_isValid = false;
		return;
	}

	glfwPollEvents();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	processInput();

	System* inputCtrlSystemPtr = SystemsHolder::getInstance().getSystem<InputControlSystem>();
	if (inputCtrlSystemPtr) {
		inputCtrlSystemPtr->process(0.1f);
	}

	const auto& gameObjects = GameObjectHolder::getInstance().getObjectsByShader();
	ShadersCtrl& shadersCtrl = ShadersCtrl::getInstance();

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	for (const auto& item : gameObjects) {
		glStencilMask(0x00);
		std::shared_ptr<Shader> shader = shadersCtrl.useProgram(item.first);

		if (shader->getProps().usesLightData) {
			setLightData(shader);
		}

		for (GameObject* ptr : item.second) {
			if (ptr->getRenderSettings().isHighlighted) {
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);
			}
			if (ptr->getRenderSettings().isVisible) {
				drawObject(shader, ptr, camera);

			}
			if (ptr->getRenderSettings().isHighlighted) {
				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);
				glDisable(GL_DEPTH_TEST);
				std::shared_ptr<Shader> highlightShader = shadersCtrl.useProgram("highlight");

				auto transform = ptr->getComponent<TransformComponent>();
				transform->setScale({ 1.2f, 1.2f, 1.2f });
				drawObject(highlightShader, ptr, camera);

				glStencilMask(0xFF);
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glEnable(GL_DEPTH_TEST);
				transform->setScale({ 1.0f, 1.0f, 1.0f });
				shadersCtrl.useProgram(item.first);
			}
		}
	}
	glfwSwapBuffers(m_window);
}

void WindowCtrl::processInput() {
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window, true);
	}
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {
		m_alpha = std::clamp(m_alpha + 0.001f, 0.0f, 1.0f);
	}
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		m_alpha = std::clamp(m_alpha - 0.001f, 0.0f, 1.0f);
	}
}

void WindowCtrl::onMousePosChanged(double x, double y) {
	//for (auto item : GameObjectHolder::getInstance().getObjectsOfType("camera")) {
	//	item->getComponent<PlayerControlsComponent>()->onMousePosChanged(x, y);
	//}
}

void WindowCtrl::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	static_cast<WindowCtrl*>(glfwGetWindowUserPointer(window))->onKeyPressed(key, action);
}
void mouseCallback(GLFWwindow* window, double x, double y) {
	static_cast<WindowCtrl*>(glfwGetWindowUserPointer(window))->onMousePosChanged(x, y);
}