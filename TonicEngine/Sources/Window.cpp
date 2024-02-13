#include "pch.hpp"
#include "framework.hpp"
#include "Window.hpp"

#include <glad/glad.hpp>
#include <GLFW/glfw3.hpp>

#include <iostream>


void FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height);

Core::Application::Window::Window(std::string _nameWindow, unsigned _width, unsigned _height)
	: width_(_width), height_(_height), nameWindow_(_nameWindow)
{
	
}

void Core::Application::Window::Init()
{
	// Initialize glfw
	if(!glfwInit())
		std::cout << "FAIL TO INITIALIZE GLFW" << std::endl;

	//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window
	p_window_ = glfwCreateWindow(width_, height_, nameWindow_.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(static_cast<GLFWwindow*>(p_window_), this);
	glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(p_window_), FramebufferResizeCallback);
	
	glfwMakeContextCurrent(static_cast<GLFWwindow*>(p_window_)); // Important to initialize GLAD

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "FAIL TO INITIALIZE GLAD" << std::endl;
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
	// Clear Color
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(static_cast<GLFWwindow*>(p_window_));
	glfwPollEvents();
}

void Core::Application::Window::Destroy()
{
	glfwDestroyWindow(static_cast<GLFWwindow*>(p_window_));
	glfwTerminate();
}