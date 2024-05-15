#include "pch.hpp"
#include "framework.hpp"
#include "Core/Window.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.hpp>

#include <iostream>

void* Core::Applications::Window::p_s_handle_ = nullptr;

static void FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height)
{
	auto window = reinterpret_cast<Core::Applications::Window*>(glfwGetWindowUserPointer(_window));
	window->Resized();
}

static void GLFWErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
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
	p_s_handle_ = (void*)glfwCreateWindow(width_, height_, nameWindow_, nullptr, nullptr);
	if (p_s_handle_ == nullptr)
	{
		glfwTerminate();
		throw ("FAILED TO CREATE GLFW WINDOW");
	}

	glfwMakeContextCurrent(static_cast<GLFWwindow*>(p_s_handle_));
	glfwSetWindowUserPointer(static_cast<GLFWwindow*>(p_s_handle_), this);
	glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(p_s_handle_), FramebufferResizeCallback);

	return p_s_handle_;
}

void* Core::Applications::Window::GetWindow() { return p_s_handle_; }

void* Core::Applications::Window::GetProcAddress()
{
	glfwMakeContextCurrent(static_cast<GLFWwindow*>(p_s_handle_)); // Important to initialize GLAD
	return (void*)glfwGetProcAddress;
}

unsigned* Core::Applications::Window::GetScreenSize()
{
	static unsigned size[2] = {width_, height_};
	//glfwGetWindowMonitor(static_cast<GLFWwindow*>(p_s_handle_));
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	size[0] = mode->width;
	size[1] = mode->height;

	return &size[0];
}

u32_2 Core::Applications::Window::GetWindowSize()
{
	return { width_, height_ };
}

void Core::Applications::Window::Resized()
{
	bFramebufferResized_ = true;
	int newWidth = 0, newHeight = 0;

	glfwGetFramebufferSize(static_cast<GLFWwindow*>(p_s_handle_), &newWidth, &newHeight);
	while (newWidth == 0 || newHeight == 0)
	{
		glfwGetFramebufferSize(static_cast<GLFWwindow*>(p_s_handle_), &newWidth, &newHeight);
		glfwWaitEvents();
	}

	width_ = static_cast<unsigned>(newWidth);
	height_ = static_cast<unsigned>(newHeight);
}

void Core::Applications::Window::ResizedHandled() { bFramebufferResized_ = false; }

bool Core::Applications::Window::IsClosing(char _state)
{
	if (_state != -1)
	{
		glfwSetWindowShouldClose(static_cast<GLFWwindow*>(p_s_handle_), _state);
	}

	return glfwWindowShouldClose(static_cast<GLFWwindow*>(p_s_handle_));
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
	glfwSwapBuffers(static_cast<GLFWwindow*>(p_s_handle_));
	glfwPollEvents();
}

void Core::Applications::Window::Destroy()
{
	glfwDestroyWindow(static_cast<GLFWwindow*>(p_s_handle_));
	glfwTerminate();
}

const float Core::Applications::Window::GetDeltaTime() const { return deltaTime_; }

const LowRenderer::Cameras::Input* Core::Applications::Window::GetFrameInput() const { return &input_; }

void Core::Applications::Window::ProcessInput()
{
	if (glfwGetKey(static_cast<GLFWwindow*>(p_s_handle_), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(static_cast<GLFWwindow*>(p_s_handle_), true);

	ProcessMouseInput();
	ProcessCameraInput();

	//Reset Offset
	input_.mouse.s_scrollOffset = 0.f;
}

//static declaration
float LowRenderer::Cameras::Input::Mouse::s_scrollOffset = 0.f;
void ScrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset)
{
	LowRenderer::Cameras::Input::Mouse::s_scrollOffset = (float)_yoffset;
};

void Core::Applications::Window::ProcessMouseInput()
{
	GLFWwindow* window = static_cast<GLFWwindow*>(p_s_handle_);

	double newMouseX, newMouseY;
	glfwGetCursorPos(window, &newMouseX, &newMouseY);
	input_.mouse.deltaX = (float)(newMouseX - input_.mouse.x);
	input_.mouse.deltaY = (float)(newMouseY - input_.mouse.y);
	input_.mouse.x = newMouseX;
	input_.mouse.y = newMouseY;

	glfwSetScrollCallback(window, ScrollCallback);
}

void Core::Applications::Window::ProcessCameraInput()
{
	GLFWwindow* window = static_cast<GLFWwindow*>(p_s_handle_);
	static double s_LastPressed = glfwGetTime();
	double timeToApply = .5;
	float mixValue = 0.2f;

	input_.cameraInput.zoom = input_.mouse.s_scrollOffset;
	// Update camera
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		input_.cameraInput.bMoveForward = glfwGetKey(window, GLFW_KEY_W);
		input_.cameraInput.bMoveBackward = glfwGetKey(window, GLFW_KEY_S);
		input_.cameraInput.bMoveUp = glfwGetKey(window, GLFW_KEY_E);
		input_.cameraInput.bMoveDown = glfwGetKey(window, GLFW_KEY_Q);
		input_.cameraInput.bMoveRight = glfwGetKey(window, GLFW_KEY_D);
		input_.cameraInput.bMoveLeft = glfwGetKey(window, GLFW_KEY_A);

		input_.cameraInput.deltaX = -input_.mouse.deltaX;
		input_.cameraInput.deltaY = input_.mouse.deltaY;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	}
	else
	{
		input_.cameraInput.bMoveForward = false;
		input_.cameraInput.bMoveBackward = false;
		input_.cameraInput.bMoveRight = false;
		input_.cameraInput.bMoveLeft = false;
		input_.cameraInput.deltaX = 0.f;
		input_.cameraInput.deltaY = 0.f;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	// Cam end
}