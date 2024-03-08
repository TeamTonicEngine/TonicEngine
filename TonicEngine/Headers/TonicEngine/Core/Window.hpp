#pragma once

#include "TonicEngine\Core\Utils\s32_2.hpp"

#include "TonicEngine\DLL_API.hpp"

namespace Core
{
	class TONIC_ENGINE_API Window
	{
	protected:
		void* p_handle_ = nullptr;

		s32 width_ = 0, height_ = 0;

		bool bFramebufferResized_ = false;

		const char* nameWindow_ = "";

	public:
		/* Input : Name of window, width of window, height of window */
		Window();

		virtual void Init(const char* _name, u32 _width, u32 _height);
		
		/* Output : Returns unsigned array size 2, width & height */
		virtual s32_2 GetWindowSize();

		/* Called when window gets resized */
		virtual void Resized();


		/* Output : Returns true if closing function has been called */
		virtual bool IsClosing();

		/* Called when window needs to be closed */
		virtual void CleanUp();
		
		/* Main loop functions go here */
		virtual void StartFrame();
		virtual void EndFrame();
		
		/* Output : Returns pointer to window (current window type is GLFWwindow) */
		void* GetHandle();
		
		/* Called when window resizing has been handled by all that need the info (Renderer, ...) */
		void ResizedHandled();
		/* Output : Returns true if window has been resized & not handled */
		bool IsFramebufferResized();



		virtual void ProcessInput();
	};
}