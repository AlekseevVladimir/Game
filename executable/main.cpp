#include <iostream>
#include <engine/render/core/graphics_ctrl.h>
#include <game/GameCtrl.h>
#include "glad/glad.h"
#include <cstdio>
#include <engine/core/systems_holder.h>
int main() {
	GraphicsCtrl graphicsCtrl;
	GameCtrl game;
	SystemsHolder::getInstance().process(1.0f);
	float lastFrameTime = glfwGetTime(), currentFrameTime = glfwGetTime();
	while (graphicsCtrl.isValid()) {
		currentFrameTime = glfwGetTime();
		graphicsCtrl.drawLoop();
		game.tick(currentFrameTime - lastFrameTime);
		lastFrameTime = currentFrameTime;

	}
	return 0;
}