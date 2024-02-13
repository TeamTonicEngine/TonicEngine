#include "EditorWindow.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <stdio.h>

#include <GLFW/glfw3.hpp>


EditorWindow::EditorWindow()
{

}

EditorWindow::~EditorWindow()
{

}

void EditorWindow::Init(void* p_window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)p_window, true);
	//ImGui_ImplOpenGL3_Init(); WAIT FOR RHI
}

void EditorWindow::CreateWindow()
{
	isClose = false;
	//ImGui::Begin("name",  &isClose);
}


void EditorWindow::GetWindow()
{

}

bool EditorWindow::IsWindowClosed()
{
	if (isClose)
		return true;
	return false;
}

void EditorWindow::CloseWindow()
{
	isClose = true;
}

void EditorWindow::Frame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
