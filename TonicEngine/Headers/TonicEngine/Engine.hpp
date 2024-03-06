#pragma once

#include "Core/Window.hpp"
#include "Core/RHI.hpp"

constexpr u32 DEFAULT_WIDTH = 1200;
constexpr u32 DEFAULT_HEIGHT = 800;

class TONIC_ENGINE_API Engine
{
protected:

	Core::Window* p_window_;
	Core::RHI* p_renderer_;

public:
	/* Input : Name of application, (optional)width of window, (optional)height of window */
	
	Engine();

	void Init(const char* _nameWindow, u32 _width, u32 _height);

	void StartFrame();
	void BindFBO();
	void RunFrame();
	void UnBindFBO();
	void EndFrame();
	void CleanUp();

	void SetWindow(Core::Window* _window);
	void SetRenderer(Core::RHI* _renderer);

	Core::Window* GetWindow();
	Core::RHI* GetRenderer();

	void RescaleFB(f32 width_, f32 height_);
	u32 GetTextureId();
};
