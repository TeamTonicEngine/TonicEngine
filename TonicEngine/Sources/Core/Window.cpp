#include "pch.hpp"
#include "Core/Window.hpp"


#include <iostream>

//void FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height);

Core::Window::Window() {}

void Core::Window::Init(const char* _name, u32 _width, u32 _height)
{

}

void* Core::Window::GetHandle()
{
	return p_handle_;
}

s32_2 Core::Window::GetWindowSize()
{
	return { 0, 0 };
}

void Core::Window::Resized()
{

}

bool Core::Window::IsClosing()
{
	return false; 
}

void Core::Window::CleanUp()
{
	
}

void Core::Window::StartFrame()
{

}

void Core::Window::EndFrame()
{
	
}


void Core::Window::ResizedHandled()
{
	bFramebufferResized_ = false;
}

bool Core::Window::IsFramebufferResized() 
{
	return bFramebufferResized_;
}

void Core::Window::ProcessInput()
{

}