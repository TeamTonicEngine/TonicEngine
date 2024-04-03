#pragma once

#include "DLL_API.hpp"
#include "LowRenderer/Camera.hpp"
#include <string>

namespace Core::Applications
{
	class TONIC_ENGINE_API Window
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
		Window(const char* _nameWindow, unsigned _width, unsigned _height);

		const bool Init();

		/* Output : Returns pointer to window (current window type is GLFWwindow) */
		void* GetWindow();
		/* Output : Returns something needed for GLAD */
		void* GetProcAddress();
		/* Output : Returns unsigned array size 2, width & height */
		unsigned* GetScreenSize();

		/* Called when window gets resized */
		void Resized();
		/* Called when window resizing has been handled by all that need the info (Renderer, ...) */
		void ResizedHandled();

		/* Output : Returns true if closing function has been called */
		bool IsClosing(char _state = -1);
		/* Output : Returns true if window has been resized & not handled */
		bool IsFramebufferResized();

		/* Start of frame */
		void StartFrame();
		/* End of frame */
		void EndFrame();
		/* Called when window needs to be closed */
		void Destroy();

		const float GetDeltaTime() const;
		const LowRenderer::Cameras::Input* GetFrameInput() const;

		void ProcessInput();
	private:
		void ProcessCameraInput();
		void ProcessMouseInput();
	};
}