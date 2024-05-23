#pragma once
#include "Core/Window.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

namespace Core::Applications
{
	class TONIC_ENGINE_API ImGuiEngine
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		void* p_window_ = nullptr;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		ImGuiEngine() = delete;
		ImGuiEngine(Core::Applications::Window* _p_window_);

		~ImGuiEngine();

		void RunGuiMouseCallback(void* _window, int button, int action, int mods);
		void RunGuiScrollCallback(void* _window, double _xoffset, double _yoffset);
		void RunGuiKeyCallback(void* _window, int _key, int _scancode, int _action, int _mods);

		/* Output : Return pointer to the current ImGuiContext */
		void* GetCurrentContext();

		/* Init Imgui for every frame */
		void StartFrame();
		/* Ends Imgui for every frame */
		void EndFrame();
	};
}