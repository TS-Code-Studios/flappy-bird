#include <iostream>

#include <caffeine-gl/base.h>

CaffeineWindow* testWindow;

int main() {
	testWindow = new CaffeineWindow();

	testWindow->createViewport();
	testWindow->setWindowTitle("Test");

	while(!glfwWindowShouldClose(testWindow->window)) {
		glfwSwapBuffers(testWindow->window);
		glfwPollEvents();
	}

	testWindow->~CaffeineWindow();
	exit(EXIT_SUCCESS);
}