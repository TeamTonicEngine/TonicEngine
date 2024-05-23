#pragma once

#include "Core/Window.hpp"
#include "Core/ImGuiEngine.hpp"

#include <Maths/Maths.hpp>
#include "Core/Utils/Type.hpp"

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

	enum class GizmoMod
	{
		TRANSLATE = 7,
		ROTATE = 56,
		SCALE = 896
	};

	class EditorWindow
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/

	protected:

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
		bool gameWindow = true;

		bool bIsSceneFocused = false;
		bool bIsGameFocused = false;

		bool bPropertyHovered = false;
		bool bSceneTreeHovered = false;
		bool bProjectHovered = false;
		bool bDebugHovered = false;
		bool bSceneHovered = false;
		bool bGameHovered = false;

		bool bOpenProjectPopup = false;

		u64 selectedId = 0;
		u64 preSelectId = 0;
		bool selectedHasTransform = false;
		Maths::Mat4 selectedTransform =
		{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
		std::string curentScenePath;
		bool gizmoUsed = false;
		GizmoMod gizmod = GizmoMod::TRANSLATE;

		/* Input : ImGuiEngine, should be init by Engine class with Engine::Init() */
		EditorWindow(ImGuiEngine* _p_imguiEngine);
		~EditorWindow();

		/* Called at start of frame to create every imgui windows */
		void SetUpWindows();

		void FocusHoveredWindow();
	private:
		/* Called at start of frame to create every imgui windows */
		void InitUIWindow();

		/* Called at start of first frame to set up the windows placements */
		void FirstFrame(ImGuiID& _mainDockSpaceId, const ImVec2& _dockspaceSize);

		Maths::Vec4 DrawTree(u64 _id);
		void DrawProperty(u64 _id);
		void DrawContentBrowser();

	public:
		/*  */
		void StartWindow(std::string _name, bool* _bOpen = nullptr);
		/* Draws window each frame */
		void DrawWindow(u64 _textureId);
	};
}