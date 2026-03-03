#include <iostream>

#include <caffeine-gl/base.hpp>

#include <game/FlappyBirdGame.hpp>

int main() {
	CaffeineWindow testWindow("Test Window");
	testWindow.createViewport();

	FlappyBirdGame flappyBird(testWindow);

	while(!testWindow.keys[GLFW_KEY_ESCAPE]) {
		flappyBird.render();
		testWindow.update();
	}

	exit(EXIT_SUCCESS);
}
