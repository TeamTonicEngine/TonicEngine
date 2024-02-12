#include "pch.hpp"
#include "framework.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.hpp>

void FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height);

Core::Application::Window::Window(std::string _nameWindow, unsigned _width, unsigned _height)
	: width_(_width), height_(_height)
{
	// Initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Create the window
	p_window_ = glfwCreateWindow(_width, _height, _nameWindow.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(static_cast<GLFWwindow*>(p_window_), this);
	glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(p_window_), FramebufferResizeCallback);
}

void FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height)
{
	auto window = reinterpret_cast<Core::Application::Window*>(glfwGetWindowUserPointer(_window));
	window->Resized();
}

void* Core::Application::Window::GetWindow()
{
	return p_window_;
}

unsigned* Core::Application::Window::GetScreenSize()
{
	static unsigned size[2] = { width_, height_ };
	return &size[0];
}

void Core::Application::Window::Resized()
{
	bFramebufferResized_ = true;
	int newWidth = 0, newHeight = 0;
	glfwGetFramebufferSize(static_cast<GLFWwindow*>(p_window_), &newWidth, &newHeight);
	while (newWidth == 0 || newHeight == 0)
	{
		glfwGetFramebufferSize(static_cast<GLFWwindow*>(p_window_), &newWidth, &newHeight);
		glfwWaitEvents();
	}
	width_ = newWidth; height_ = newHeight;
}

void Core::Application::Window::ResizedHandled()
{
	bFramebufferResized_ = false;
}

bool Core::Application::Window::IsClosing()
{
	return glfwWindowShouldClose(static_cast<GLFWwindow*>(p_window_));
}

bool Core::Application::Window::IsFramebufferResized()
{
	return bFramebufferResized_;
}

void Core::Application::Window::Loop()
{
	glfwPollEvents();
}

void Core::Application::Window::Destroy()
{
	glfwDestroyWindow(static_cast<GLFWwindow*>(p_window_));
	glfwTerminate();
}