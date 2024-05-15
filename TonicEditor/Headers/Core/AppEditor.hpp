#pragma once

#include "Core/Engine.hpp"
#include "EditorWindow.hpp"

namespace LowRenderer::Cameras
{
	class FreeCamera;
}
struct Buffer;

namespace Core::Applications
{
	class AppEditor
	{
	protected:
		/**********************************************
				VARIABLES BLOC
		**********************************************/
		Engine* p_engine_ = nullptr;
		EditorWindow* p_editorWindow_ = nullptr;
		LowRenderer::Cameras::FreeCamera* p_freeCamera_;
		WindowBuffer* p_sceneFBO;
		WindowBuffer* p_gameFBO;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		AppEditor(std::string _appName, u32 _width = DEFAULT_WIDTH, u32 _height = DEFAULT_HEIGHT);
		~AppEditor();

		void Init();

		void PlayScene();
		void PlayGame();

		/* Main loop of Tonic Editor Program */
		void Run();
	};
}