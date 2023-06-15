#pragma once
#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <engine/core/window_ctrl.h>
#include <engine/render/open_gl/shaders/solid_object_shader.h>
#include <engine/core/game_object.h>
#include <memory>
#include <glfw3.h>
//#include <engine/render/core/shaders_ctrl.h>
//#include <Model.h>

// TODO move cod to cpp
class GraphicsCtrl 
{
public:
	GraphicsCtrl() 
	{
		glfwInit();
		WindowCtrl& windowCtrl = WindowCtrl::getInstance();
		windowCtrl.init();
		bool res = windowCtrl.isValid();
		if (!res)
			return;
		if (!res)
			return;

		m_isValid = true;
	}

	void drawLoop() 
	{
		if (!m_isValid) 
		{
			std::cerr << "Cannot start draw: not properly configured" << std::endl;
			return;
		}
		WindowCtrl& windowCtrl = WindowCtrl::getInstance();

		windowCtrl.draw();
		m_isValid = windowCtrl.isValid();
	}

	bool isValid() 
	{
		return m_isValid;
	}

	~GraphicsCtrl() 
	{
		glfwTerminate();
	}
private:
	bool m_isValid = false;
};
