#pragma once

#include "TonicEngine/Core/Window.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

enum EditorWindowPos
{
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	CENTER
};


class EditorWindow
{
private:
	const ImGuiViewport* viewport;
	bool isClose = true;
	void* p_window_;

	EditorWindowPos windowPos_;

public:
	float window_width, window_height;
	ImVec2 pos;

	EditorWindow();
	~EditorWindow();

	void SetupImGui(Core::Window* p_window);

	void GetWindow();
	bool IsWindowClosed();
	void CloseWindow();

	void StartFrame();
	void SetUpWindows();
	void InitUIWindow();

	void Test();

	void StartWindow();
	void EndWindow(unsigned int texture_id);

	void EndFrame();

	void CleanUp();


};


