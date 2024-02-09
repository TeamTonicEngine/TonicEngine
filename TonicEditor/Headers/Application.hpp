#pragma once

#include "Window.hpp"

namespace Core
{
	constexpr unsigned defaultWidth_CONST = 1200;
	constexpr unsigned defaultHeight_CONST = 800;

	class Application
	{
	private:
		const uint32_t width_;
		const uint32_t height_;

		std::string appName_;

		Window window_;
		//RHI renderer_;

	public:
		Application(std::string _appName, uint32_t _width = defaultWidth_CONST, uint32_t _height = defaultHeight_CONST)
			: appName_(_appName), width_(_width), height_(_height),
			window_(_appName, _width, _height)
		{};

		inline void Run()
		{
			while (!window_.IsClosing())
			{
				window_.Loop();

				if (window_.IsFramebufferResized())
				{
					// renderer

					window_.ResizedHandled();
				}
				else
				{
					// renderer
				}
			}

			window_.Destroy();
		}
	};
}