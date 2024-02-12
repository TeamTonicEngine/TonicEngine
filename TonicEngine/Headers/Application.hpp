#pragma once

#include "Window.hpp"

namespace Core::Application
{
	constexpr unsigned defaultWidth_CONST = 1200;
	constexpr unsigned defaultHeight_CONST = 800;

	class TONIC_ENGINE_API Application
	{
	protected:
		uint32_t width_;
		uint32_t height_; // TODO change to vec2 int ??

		const char* appName_;

		Window* p_window_;
		//RHI* p_renderer_;

	public:
		Application(std::string _appName, uint32_t _width = defaultWidth_CONST, uint32_t _height = defaultHeight_CONST)
			: appName_(_appName.c_str()), width_(_width), height_(_height)
		{
			p_window_ = new Window(_appName, _width, _height);
		};

		inline void Run()
		{
			while (!p_window_->IsClosing())
			{
				p_window_->Loop();

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

			p_window_->Destroy();
		}
	};
}