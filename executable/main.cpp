#include <iostream>
#include <engine/render/core/graphics_ctrl.h>
#include <GameCtrl.h>
#include "Render/stb_image.h"
#include "glad/glad.h"
#include <cstdio>
int main() {
	GraphicsCtrl graphicsCtrl;
	GameCtrl game;
	float lastFrameTime = glfwGetTime(), currentFrameTime = glfwGetTime();
	while (graphicsCtrl.isValid()) {
		currentFrameTime = glfwGetTime();
		graphicsCtrl.drawLoop();
		game.tick(currentFrameTime - lastFrameTime);
		lastFrameTime = currentFrameTime;

	}
	return 0;
}