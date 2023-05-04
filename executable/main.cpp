#include <iostream>
#include <engine/render/core/graphics_ctrl.h>
#include <game/GameCtrl.h>
#include "glad/glad.h"
#include <cstdio>
#include <engine/core/systems_holder.h>
int main() {
	GraphicsCtrl graphicsCtrl;
	GameCtrl game;
	//SystemsHolder::getInstance().process(1.0f);
	float lastFrameTime = glfwGetTime(), currentFrameTime = glfwGetTime();
	while (graphicsCtrl.isValid()) {
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		currentFrameTime = glfwGetTime();
		game.tick(currentFrameTime - lastFrameTime);
		graphicsCtrl.drawLoop();
		lastFrameTime = currentFrameTime;

	}
	return 0;
}