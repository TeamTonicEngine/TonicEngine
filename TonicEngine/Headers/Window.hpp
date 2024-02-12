#pragma once

#include "DLL_API.hpp"

#include <string>

namespace Core::Application
{
	class TONIC_ENGINE_API Window
	{
	private:
		void* p_window_ = nullptr;

		unsigned width_ = 0, height_ = 0;

		bool bFramebufferResized_ = false;

	public:
		Window(std::string _nameWindow, unsigned _width, unsigned _height);

		void* GetWindow();
		unsigned* GetScreenSize(); // TODO change to vec2 int when math lib implemented

		void Resized();
		void ResizedHandled();

		bool IsClosing();
		bool IsFramebufferResized();

		void Loop();
		void Destroy();
	};
}