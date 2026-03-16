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

	FlappyBirdGame flappyBird(window);

	flappyBird.init();

	while(!window.keys[GLFW_KEY_ESCAPE]) {
		currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		flappyBird.update(deltaTime);
		CollisionSystem::update(deltaTime);
		flappyBird.render();
		window.update();
	}

	return 0;
}
