#include "Core/EngineOverride/WindowOverride.hpp"

#include <stdexcept>
#include <iostream>

#include <GLFW/glfw3.hpp>
#include "TonicEngine/Core/Log.hpp"

void FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height)
{
	auto window = reinterpret_cast<Core::Window*>(glfwGetWindowUserPointer(_window));
	window->Resized();
}

WindowOverride::WindowOverride() {}

void WindowOverride::Init(const char* _name, u32 _width, u32 _height)
{
	// Initialize glfw
	if (!glfwInit())
		throw std::exception("FAIL TO INITIALIZE GLFW");
	else
		DEBUG_SUCCESS("SUCCEED TO INITIALIZE GLFW");

	// Create the window
	p_handle_ = glfwCreateWindow(_width, _height, _name, nullptr, nullptr);

	if (!p_handle_)
		throw std::exception("FAIL TO CREATE GLFW WINDOW");
	else
		DEBUG_SUCCESS("SUCCEED TO CREATE GLFW WINDOW");

	glfwSetWindowUserPointer(static_cast<GLFWwindow*>(p_handle_), this);
	glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(p_handle_), FramebufferResizeCallback);

	glfwMakeContextCurrent(static_cast<GLFWwindow*>(p_handle_));
}

s32_2 WindowOverride::GetWindowSize()
{
	s32 width, height;
	glfwGetWindowSize(static_cast<GLFWwindow*>(p_handle_), &width, &height);
	return { width , height };
}

void WindowOverride::Resized()
{
	bFramebufferResized_ = true;
	int newWidth, newHeight;
	glfwGetFramebufferSize(static_cast<GLFWwindow*>(p_handle_), &newWidth, &newHeight);
	while (newWidth == 0 || newHeight == 0)
	{
		glfwGetFramebufferSize(static_cast<GLFWwindow*>(p_handle_), &newWidth, &newHeight);
		glfwWaitEvents();
	}
}

bool WindowOverride::IsClosing()
{
	return glfwWindowShouldClose(static_cast<GLFWwindow*>(p_handle_));
}

void WindowOverride::CleanUp()
{
	glfwDestroyWindow(static_cast<GLFWwindow*>(p_handle_));
	glfwTerminate();
}

void WindowOverride::StartFrame()
{
}

void WindowOverride::EndFrame()
{
	glfwSwapBuffers(static_cast<GLFWwindow*>(p_handle_));
	glfwPollEvents();
}

void WindowOverride::ProcessInput()
{
	if (glfwGetKey(static_cast<GLFWwindow*>(p_handle_), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(static_cast<GLFWwindow*>(p_handle_), true);
}