#include "pch.hpp"

#include "framework.hpp"
#include "Core/Window.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.hpp>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.hpp>

#include <iostream>

void* Core::Applications::Window::s_p_handle_ = nullptr;

static void FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height)
{
	auto window = reinterpret_cast<Core::Applications::Window*>(glfwGetWindowUserPointer(_window));
	window->Resized();
}

static void GLFWErrorCallback(int _error, const char* _description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", _error, _description);
}

Core::Applications::Window::Window(const char* _nameWindow, unsigned _width, unsigned _height) :
	width_(_width), height_(_height), nameWindow_(_nameWindow)
{}

const bool Core::Applications::Window::Init()
{
	glfwSetErrorCallback(GLFWErrorCallback);

	// Initialize glfw
	if (!glfwInit())
		throw ("FAILED TO INITIALIZE GLFW");
	else
		DEBUG_SUCCESS("INITIALIZED GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Anti-aliasing
	glfwWindowHint(GLFW_SAMPLES, 4);
	// Create the window
	s_p_handle_ = (void*)glfwCreateWindow(width_, height_, nameWindow_, nullptr, nullptr);
	if (s_p_handle_ == nullptr)
	{
		glfwTerminate();
		throw ("FAILED TO CREATE GLFW WINDOW");
	}

	glfwMakeContextCurrent(static_cast<GLFWwindow*>(s_p_handle_));
	glfwSetWindowUserPointer(static_cast<GLFWwindow*>(s_p_handle_), this);
	glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(s_p_handle_), FramebufferResizeCallback);

	return s_p_handle_;
}

void* Core::Applications::Window::GetWindow() { return s_p_handle_; }

void* Core::Applications::Window::GetProcAddress()
{
	glfwMakeContextCurrent(static_cast<GLFWwindow*>(s_p_handle_)); // Important to initialize GLAD
	return (void*)glfwGetProcAddress;
}

unsigned* Core::Applications::Window::GetScreenSize()
{
	static unsigned size[2] = { width_, height_ };
	//glfwGetWindowMonitor(static_cast<GLFWwindow*>(s_p_handle_));
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	size[0] = mode->width;
	size[1] = mode->height;

	return &size[0];
}

u32_2 Core::Applications::Window::GetWindowSize() { return { width_, height_ }; }

f32_2 TONIC_ENGINE_API Core::Applications::Window::GetWindowPos()
{
	int x, y;
	glfwGetWindowPos(static_cast<GLFWwindow*>(s_p_handle_), &x, &y);
	return { (float)x,(float)y };
}

void Core::Applications::Window::Resized()
{
	bFramebufferResized_ = true;
	int newWidth = 0, newHeight = 0;

	glfwGetFramebufferSize(static_cast<GLFWwindow*>(s_p_handle_), &newWidth, &newHeight);
	while (newWidth == 0 || newHeight == 0)
	{
		glfwGetFramebufferSize(static_cast<GLFWwindow*>(s_p_handle_), &newWidth, &newHeight);
		glfwWaitEvents();
	}

	width_ = static_cast<unsigned>(newWidth);
	height_ = static_cast<unsigned>(newHeight);
}

void Core::Applications::Window::ResizedHandled() { bFramebufferResized_ = false; }

bool Core::Applications::Window::IsClosing(char _state)
{
	if (_state != -1)
		glfwSetWindowShouldClose(static_cast<GLFWwindow*>(s_p_handle_), _state);

	return glfwWindowShouldClose(static_cast<GLFWwindow*>(s_p_handle_));
}

bool Core::Applications::Window::IsFramebufferResized() { return bFramebufferResized_; }

void Core::Applications::Window::StartFrame()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime_ = currentFrame - lastFrame_;
	lastFrame_ = currentFrame;
}

void Core::Applications::Window::EndFrame()
{
	glfwSwapBuffers(static_cast<GLFWwindow*>(s_p_handle_));
	glfwPollEvents();
}

void Core::Applications::Window::Destroy()
{
	glfwDestroyWindow(static_cast<GLFWwindow*>(s_p_handle_));
	glfwTerminate();
}

const float Core::Applications::Window::GetDeltaTime() const { return deltaTime_; }

void Core::Applications::Window::RegisterInputHandler()
{
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Keyboard::KeyEscape, [&]() {IsClosing(true); });
}

void Core::Applications::Window::SetFullScreen(bool _state)
{
	if (_state)
	{
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
		glfwSetWindowMonitor(static_cast<GLFWwindow*>(s_p_handle_), primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else
	{
		glfwSetWindowMonitor(static_cast<GLFWwindow*>(s_p_handle_), nullptr, GetScreenSize()[0] * 0.5f - DEFAULT_WIDTH * 0.5f, GetScreenSize()[1] * 0.5f - DEFAULT_HEIGHT * 0.5f, DEFAULT_WIDTH, DEFAULT_HEIGHT, 0);
	}
}

void* Core::Applications::Window::GetWindowsHwnd()
{
	return glfwGetWin32Window(static_cast<GLFWwindow*>(s_p_handle_));
}