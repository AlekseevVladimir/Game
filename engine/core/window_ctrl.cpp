#include "window_ctrl.h"
#include <engine/player_controls/player_controls_component.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <engine/utils/engine_constants.h>
#include <engine/core/game_object.h>
#include <utility>
#include <glfw3.h>
#include <glad/glad.h>
#include <engine/core/systems_holder.h>
#include <engine/player_controls/input_control_system.h>

void WindowCtrl::init() 
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);


	m_window = glfwCreateWindow(RES_WIDTH, RES_HEIGHT, "First OpenGL app", NULL, NULL);
	m_height = RES_HEIGHT;
	m_width = RES_WIDTH;
	if (m_window == NULL) 
	{
		std::cerr << "Failed to create window" << std::endl;
		return;
	}
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	m_isValid = true;
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return;
	}
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetKeyCallback(m_window, keyCallback);
	glfwSetCursorPosCallback(m_window, mouseCallback);
	glfwSetWindowUserPointer(m_window, this);
}

void WindowCtrl::setWindowDimensions(int width, int height)
{
	m_height = height;
	m_width = width;
	glViewport(0, 0, width, height);
}

void WindowCtrl::draw() 
{
	if (glfwWindowShouldClose(m_window) || !m_isValid) 
	{
		m_isValid = false;
		return;
	}

	glfwPollEvents();

	processInput();

	System* inputCtrlSystemPtr = SystemsHolder::getInstance().getSystem<InputControlSystem>();
	if (inputCtrlSystemPtr) 
	{
		inputCtrlSystemPtr->process(0.1f);
	}

	const auto& gameObjects = GameObjectHolder::getInstance().getObjectsByShader();

	glfwSwapBuffers(m_window);
}

void WindowCtrl::processInput() {
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(m_window, true);
	}
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) 
	{
		m_alpha = std::clamp(m_alpha + 0.001f, 0.0f, 1.0f);
	}
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) 
	{
		m_alpha = std::clamp(m_alpha - 0.001f, 0.0f, 1.0f);
	}
}

void WindowCtrl::onMousePosChanged(double x, double y) 
{
	//for (auto item : GameObjectHolder::getInstance().getObjectsOfType("camera")) {
	//	item->getComponent<PlayerControlsComponent>()->onMousePosChanged(x, y);
	//}
}

void WindowCtrl::framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	static_cast<WindowCtrl*>(glfwGetWindowUserPointer(window))->setWindowDimensions(width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	static_cast<WindowCtrl*>(glfwGetWindowUserPointer(window))->onKeyPressed(key, action);
}
void mouseCallback(GLFWwindow* window, double x, double y) 
{
	static_cast<WindowCtrl*>(glfwGetWindowUserPointer(window))->onMousePosChanged(x, y);
}