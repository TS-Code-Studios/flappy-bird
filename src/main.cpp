#include <iostream>

#include <caffeine-gl/base.hpp>

#include <game/FlappyBirdGame.hpp>

float lastFrame = 0.0;
float currentFrame = 0.0;
float deltaTime = 0.0;

int main() {
	CaffeineWindow testWindow("Test Window");
	testWindow.createViewport();

	FlappyBirdGame flappyBird(testWindow);

	flappyBird.init();

	while(!testWindow.keys[GLFW_KEY_ESCAPE]) {
		currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		flappyBird.update(deltaTime);
		flappyBird.render();
		testWindow.update();
	}

	return 0;
}
