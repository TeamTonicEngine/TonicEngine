#include "Window.hpp"
#include <GLFW/glfw3.hpp>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.hpp>

Window::Window() : handle(nullptr) {}
Window::~Window() {}

void* Window::GetWindowsHwnd()
{
	return glfwGetWin32Window(static_cast<GLFWwindow*>(handle));
}

void Window::Init(int _width, int _height, const char* _title)
{
	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	handle = glfwCreateWindow(_width, _height, _title, NULL, NULL);

	if (!handle)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(static_cast<GLFWwindow*>(handle));


	glClearColor(0.1921, 0.2000, 0.2196, 1.0000);
}

void Window::StartFrame()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::EndFrame()
{
	glfwSwapBuffers(static_cast<GLFWwindow*>(handle));
	glfwPollEvents();
}

void Window::CleanUp()
{
	glfwTerminate();
}

int Window::WindowShouldClose()
{
	return glfwWindowShouldClose(static_cast<GLFWwindow*>(handle));
}

void Window::Close()
{
	glfwSetWindowShouldClose(static_cast<GLFWwindow*>(handle),true);
}