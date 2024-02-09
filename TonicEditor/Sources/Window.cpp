#include "Window.hpp"

#include <GLFW/glfw3.h>

void FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height);

Core::Window::Window(std::string _nameWindow, unsigned _width, unsigned _height)
	: width_(_width), height_(_height)
{
	// Initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Create the window
	window_ = glfwCreateWindow(_width, _height, _nameWindow.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(static_cast<GLFWwindow*>(window_), this);
	glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(window_), FramebufferResizeCallback);
}

void FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height)
{
	auto window = reinterpret_cast<Core::Window*>(glfwGetWindowUserPointer(_window));
	window->Resized();
}

void* Core::Window::GetWindow() const
{
	return static_cast<GLFWwindow*>(window_);
}

unsigned* Core::Window::GetScreenSize() const
{
	unsigned size[2] = { width_, height_ };
	return size;
}

void Core::Window::Resized()
{
	bFramebufferResized_ = true;
	int newWidth = 0, newHeight = 0;
	glfwGetFramebufferSize(static_cast<GLFWwindow*>(window_), &newWidth, &newHeight);
	while (newWidth == 0 || newHeight == 0)
	{
		glfwGetFramebufferSize(static_cast<GLFWwindow*>(window_), &newWidth, &newHeight);
		glfwWaitEvents();
	}
	width_ = newWidth; height_ = newHeight;
}

void Core::Window::ResizedHandled()
{
	bFramebufferResized_ = false;
}

bool Core::Window::IsClosing() const
{
	return glfwWindowShouldClose(static_cast<GLFWwindow*>(window_));
}

bool Core::Window::IsFramebufferResized() const
{
	return bFramebufferResized_;
}

void Core::Window::Loop()
{
	glfwPollEvents();
}

void Core::Window::Destroy()
{
	glfwDestroyWindow(static_cast<GLFWwindow*>(window_));
	glfwTerminate();
}