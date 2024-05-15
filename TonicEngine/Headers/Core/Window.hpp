#pragma once

#include "DLL_API.hpp"
#include "LowRenderer/Cameras/FreeCamera.hpp"
#include "Utils/u32_2.hpp"
#include <string>

namespace Core::Applications
{
	class Window
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		static void* p_s_handle_;

		unsigned width_ = 0, height_ = 0;

		bool bFramebufferResized_ = false;

		const char* nameWindow_ = "";

		LowRenderer::Cameras::Input input_;

		float deltaTime_ = 0.0f;
		float lastFrame_ = 0.0f;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		/* Input : Name of window, width of window, height of window */
		TONIC_ENGINE_API Window(const char* _nameWindow, unsigned _width, unsigned _height);

		const bool TONIC_ENGINE_API Init();

		/* Output : Returns pointer to window (current window type is GLFWwindow) */
		void* GetWindow();
		/* Output : Returns something needed for GLAD */
		void* GetProcAddress();
		/* Output : Returns unsigned array size 2, width & height */
		TONIC_ENGINE_API unsigned* GetScreenSize();
		u32_2 TONIC_ENGINE_API GetWindowSize();

		/* Called when window gets resized */
		void TONIC_ENGINE_API Resized();
		/* Called when window resizing has been handled by all that need the info (Renderer, ...) */
		void TONIC_ENGINE_API ResizedHandled();

		/* Output : Returns true if closing function has been called */
		bool TONIC_ENGINE_API IsClosing(char _state = -1);
		/* Output : Returns true if window has been resized & not handled */
		bool TONIC_ENGINE_API IsFramebufferResized();

		/* Start of frame */
		void TONIC_ENGINE_API StartFrame();
		/* End of frame */
		void TONIC_ENGINE_API EndFrame();
		/* Called when window needs to be closed */
		void TONIC_ENGINE_API Destroy();

		const float TONIC_ENGINE_API GetDeltaTime() const;
		const LowRenderer::Cameras::Input* GetFrameInput() const;

		void TONIC_ENGINE_API ProcessInput();
	private:
		void TONIC_ENGINE_API ProcessCameraInput();
		void TONIC_ENGINE_API ProcessMouseInput();
	};
}