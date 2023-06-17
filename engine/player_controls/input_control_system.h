#pragma once

#include <vector>
#include <engine/core/system.h>
#include <engine/core/game_object.h>

class GLFWwindow;

class InputControlSystem : public System 
{
public:
	InputControlSystem() {}

	void process(float delta) override 
	{
		processInput();
		processMousePos();

	}

	void processInput();

	void processMousePos();

private:
	bool m_firstMouse = true;
	float m_lastX = 0.0f;
	float m_lastY = 0.0f;
};
