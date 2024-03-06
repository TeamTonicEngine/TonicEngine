#pragma once

#include "TonicEngine/Engine.hpp"
#include "EditorWindow.hpp"
#include"Core/EngineOverride/WindowOverride.hpp"
#include"Core/EngineOverride/GL_RHI.hpp"

namespace Core::Applications
{
	constexpr u32 DEFAULT_WIDTH = 1200;
	constexpr u32 DEFAULT_HEIGHT = 800;

	class AppEditor
	{
	protected:
		Engine* p_engine_;
		EditorWindow* p_editorWindow_;
		WindowOverride* p_windowOW_;
		RHI* p_Renderer_;
	public:
		
		AppEditor(const char* _appName, u32 _width = DEFAULT_WIDTH, u32 _height = DEFAULT_HEIGHT);
		~AppEditor();
		void Run();
		
	};
}
