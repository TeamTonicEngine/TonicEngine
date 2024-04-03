#pragma once

#include "Core/Window.hpp"

#include "Core/ImGuiEngine.hpp"

#include <Maths/Maths.hpp>

namespace Core::Applications
{
	enum class EditorWindowPos
	{
		Top,
		Bottom,
		Left,
		Right,
		Center,
		Unset
	};

	class EditorWindow
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		EditorWindowPos windowPos_ = EditorWindowPos::Unset;

		ImGuiEngine* p_imguiEngine_ = nullptr;
		ImGuiViewport* p_viewport_ = nullptr;
		void* p_window_ = nullptr;

		float width_ = 0.f, height_ = 0.f;
		ImVec2 pos_;

		void* p_projectArchi_;

		float fpsHistory_[100] = { 0.f };

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		/* Input : ImGuiEngine, should be init by Engine class with Engine::Init() */
		EditorWindow(ImGuiEngine* _p_imguiEngine);
		~EditorWindow();

		/* Called at start of frame to create every imgui windows */
		void SetUpWindows();
	private:
		/* Called at start of frame to create every imgui windows */
		void InitUIWindow();

		/* Called at start of first frame to set up the windows placements */
		void FirstFrame(ImGuiID& _mainDockSpaceId, const ImVec2& _dockspaceSize);

	public:
		/*  */
		void StartWindow();
		/* Draws window each frame */
		void DrawWindow(unsigned _textureId);
	};
}