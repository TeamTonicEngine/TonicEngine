#pragma once

#include "Core/Engine.hpp"
#include "EditorWindow.hpp"

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

		LowRenderer::Cameras::Input input_;

		WindowBuffer* p_sceneFBO_;
		WindowBuffer* p_gameFBO_;

		bool bGame = false;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		AppEditor(std::string _appName, u32 _width = DEFAULT_WIDTH, u32 _height = DEFAULT_HEIGHT);
		~AppEditor();

		void UpdateCurrentCamera();

		void Init();

		void  HandleEvent();

		void PlayScene();
		void PlayGame();

		/* Main loop of Tonic Editor Program */
		void Run();

	private:
		void RegisterScripts();
	};
}