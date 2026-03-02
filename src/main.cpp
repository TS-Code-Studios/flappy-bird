#include <iostream>

#include <caffeine-gl/base.h>

int WIDTH;
int HEIGHT;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main() {
	glfwSetKeyCallback(key_callback);

	if(!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

	WIDTH = videoMode->width;
	HEIGHT = videoMode->height;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}