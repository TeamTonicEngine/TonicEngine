#include <pch.hpp>

#include "Core/RHI.hpp"

const bool Core::Renderer::RHI::Init(Core::Applications::Window* _p_window)
{
	p_window_ = _p_window;
	// Here in child add any additional function to link the window to specific RHI
	return true;
}

void Core::Renderer::RHI::SetCamera(LowRenderer::Cameras::FreeCamera* _p_newCamera) { p_currentCamera_ = _p_newCamera; }