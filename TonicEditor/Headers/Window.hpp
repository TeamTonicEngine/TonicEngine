#pragma once

#include <string>

namespace Core
{
	class Window
	{
	private:
		void* window_ = nullptr;

		unsigned width_ = 0, height_ = 0;

		bool bFramebufferResized_ = false;

	public:
		Window(std::string _nameWindow, unsigned _width, unsigned _height);

		void* GetWindow() const;
		unsigned* GetScreenSize() const; // TODO change to vec2 int when math lib implemented

		void Resized();
		void ResizedHandled();

		bool IsClosing() const;
		bool IsFramebufferResized() const;

		void Loop();
		void Destroy();
	};
}