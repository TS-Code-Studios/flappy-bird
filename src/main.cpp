#include <iostream>

#include <caffeine-gl/base.hpp>

#include <game/FlappyBirdGame.hpp>

float lastFrame = 0.0;
float currentFrame = 0.0;
float deltaTime = 0.0;

int main() {
	CaffeineWindow window("Flappy Bird");
	window.createViewport();
	window.toggleFullscreen();

	CaffeineWorld world;
	FlappyBirdGame flappyBird(window, world);

	flappyBird.init();

	while(!window.keys[GLFW_KEY_ESCAPE]) {
		currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		flappyBird.update(deltaTime);
		flappyBird.render();
		window.update();
	}
	world.clear();
	CaffeineResourceManager::clear();
	return 0;
}
