#pragma once

#include "DLL_API.hpp"

#include <string>
namespace Core
{
	class Window
	{
	private:
		void* p_window_ = nullptr;

		unsigned width_ = 0, height_ = 0;

		bool bFramebufferResized_ = false;

	public:
		extern "C" TONIC_ENGINE_API Window(std::string _nameWindow, unsigned _width, unsigned _height);

		extern "C" TONIC_ENGINE_API void* GetWindow() const;
		extern "C" TONIC_ENGINE_API unsigned* GetScreenSize() const; // TODO change to vec2 int when math lib implemented

		extern "C" TONIC_ENGINE_API void Resized();
		extern "C" TONIC_ENGINE_API void ResizedHandled();

		extern "C" TONIC_ENGINE_API bool IsClosing() const;
		extern "C" TONIC_ENGINE_API bool IsFramebufferResized() const;

		extern "C" TONIC_ENGINE_API void Loop();
		extern "C" TONIC_ENGINE_API void Destroy();
	};
}