#include <iostream>

#include <caffeine-gl/base.hpp>

int main() {
	const CaffeineWindow testWindow("Test Window");

	testWindow.createViewport();

	while(!testWindow.keys[GLFW_KEY_ESCAPE]) {
		testWindow.update();
	}

	exit(EXIT_SUCCESS);
}