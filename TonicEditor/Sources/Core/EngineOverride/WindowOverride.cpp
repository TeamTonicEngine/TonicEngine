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
	freeCamera_ = new FreeCamera(_width, _height);
	freeCamera_->bUsed = true;

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
	glfwSetInputMode(static_cast<GLFWwindow*>(p_handle_), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

s32_2 WindowOverride::GetWindowSize()
{
	glfwGetWindowSize(static_cast<GLFWwindow*>(p_handle_), &width_, &height_);
	return { width_ , height_ };
}

FreeCamera* WindowOverride::GetCamera()
{
	return freeCamera_;
}

void WindowOverride::Resized()
{
	bFramebufferResized_ = true;
	glfwGetFramebufferSize(static_cast<GLFWwindow*>(p_handle_), &width_, &height_);
	while (width_ == 0 || height_ == 0)
	{
		glfwGetFramebufferSize(static_cast<GLFWwindow*>(p_handle_), &width_, &height_);
		
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
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
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

	ProcessMouseInput();

	ProcessCameraInput();
	//Reset Offset
	input_.mouse.s_scrollOffset = 0.f;

}

//static declaration
float Input::Mouse::s_scrollOffset = 0.f;
void WindowOverride::ScrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset) 
{
	Input::Mouse::s_scrollOffset = (float)_yoffset;
};


void WindowOverride::ProcessMouseInput()
{
	GLFWwindow* window = static_cast<GLFWwindow*>(p_handle_);

	double newMouseX, newMouseY;
	glfwGetCursorPos(window, &newMouseX, &newMouseY);
	input_.mouse.deltaX = (float)(newMouseX - input_.mouse.x);
	input_.mouse.deltaY = (float)(newMouseY - input_.mouse.y);
	input_.mouse.x = newMouseX;
	input_.mouse.y = newMouseY;

	glfwSetScrollCallback(window, ScrollCallback);
}


void WindowOverride::ProcessCameraInput()
{
	GLFWwindow* window = static_cast<GLFWwindow*>(p_handle_);
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