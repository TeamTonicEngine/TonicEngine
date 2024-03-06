#include "pch.hpp"
#include <Engine.hpp>

#include <iostream>

using namespace Core;

Engine::Engine() : p_window_(nullptr), p_renderer_(nullptr){}

void Engine::Init(const char* _nameWindow, u32 _width, u32 _height)
{
	if (!p_window_)
	{
		std::cerr << "Window NULL\n";
		return;
	}
	p_window_->Init(_nameWindow, _width, _height);


	if (!p_renderer_)
	{
		std::cerr << "Renderer NULL\n";
		return;
	}
	p_renderer_->Init(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	p_renderer_->InitShader();
	p_renderer_->InitFrameBuffer();
}


void Engine::StartFrame()
{
	p_window_->StartFrame();
	p_renderer_->StartFrame();
	p_window_->ProcessInput();

	if (p_window_->IsFramebufferResized())
	{
		// renderer

		p_window_->ResizedHandled();
	}
	else
	{
		// renderer
	}
}

void Engine::RunFrame()
{
	p_renderer_->BindFrameBuffer();
	p_renderer_->DrawTriangle();
	RescaleFB(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	p_renderer_->UnbindFrameBuffer();
}


void Engine::EndFrame()
{
	p_window_->EndFrame();
}


void Engine::CleanUp()
{
	p_window_->CleanUp();
	p_renderer_->CleanUp();
	
}

Core::Window* Engine::GetWindow() { return p_window_; }
Core::RHI* Engine::GetRenderer() { return p_renderer_; }

void Engine::SetWindow(Core::Window* _window) { p_window_ = _window; }
void Engine::SetRenderer(Core::RHI* _renderer) { p_renderer_ = _renderer; }





void Engine::RescaleFB(float width_, float height_)
{
	p_renderer_->RescaleFrameBuffer(width_, height_);
	
}

unsigned int Engine::GetTextureId()
{
	return p_renderer_->GetTextureID();
}

