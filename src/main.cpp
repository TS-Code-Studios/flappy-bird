#include <iostream>

#include <caffeine-gl/base.hpp>

#include <game/FlappyBirdGame.hpp>

int main() {
	CaffeineWindow testWindow("Test Window");
	testWindow.createViewport();

	FlappyBirdGame flappyBird(testWindow);

	flappyBird.init();

	while(!testWindow.keys[GLFW_KEY_ESCAPE]) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		flappyBird.render();
		testWindow.update();

	}

	exit(EXIT_SUCCESS);
}
