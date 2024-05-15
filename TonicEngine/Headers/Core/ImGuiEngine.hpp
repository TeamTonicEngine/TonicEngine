#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include "Core/Window.hpp"

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

		/* Output : Return pointer to the current ImGuiContext */
		void* GetCurrentContext();

		/* Init Imgui for every frame */
		void StartFrame();
		/* Ends Imgui for every frame */
		void EndFrame();
	};
}