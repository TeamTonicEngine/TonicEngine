#include <iostream>

//#include "glad.h"
#include "GLFW/glfw3.h"

#include "TonicEngine/Test.hpp"

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

int main()
{
	// Run the application
	// -------------------
	GLFWwindow* window = InitWindow();

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		Update();
	}
    return 0;
}

void Update()
{
	// Update the application
	// ----------------------

}

GLFWwindow* InitWindow()
{
	GLFWwindow* window;

	// Initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Create the window
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(window, NULL);

	return window;
}
