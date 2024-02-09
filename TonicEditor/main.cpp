#include <iostream>
#include "TonicEngine/Test.hpp"
#include <glad/glad.hpp>
#include <GLFW/glfw3.h>
//#include "glad.h"


const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

void Update();
GLFWwindow* InitWindow();

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
