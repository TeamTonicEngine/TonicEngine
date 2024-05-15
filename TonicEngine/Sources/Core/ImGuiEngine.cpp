#include "pch.hpp"

#include "Core/ImGuiEngine.hpp"

#include <GLFW/glfw3.hpp>

Core::Applications::ImGuiEngine::ImGuiEngine(Core::Applications::Window* _p_window_)
{
	p_window_ = _p_window_->GetWindow();
	glfwMakeContextCurrent((GLFWwindow*)p_window_);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // IF using Docking Branch
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	// Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)(p_window_), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

Core::Applications::ImGuiEngine::~ImGuiEngine()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void* Core::Applications::ImGuiEngine::GetCurrentContext() { return ImGui::GetCurrentContext(); }

void Core::Applications::ImGuiEngine::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Core::Applications::ImGuiEngine::EndFrame()
{
	ImGui::Render();
	
	u32* display = ENGINE.WDW->GetScreenSize();

	Engine::GetRenderer()->ResizeViewPort(display[0], display[1]);
	//Engine::GetRenderer()->ClearColor();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}


	//glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	ImGui::EndFrame();

}