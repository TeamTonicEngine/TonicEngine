#pragma once

#include "Core/Utils/Type.hpp"

#include "Core/Window.hpp"
#include "Core/RHI.hpp"
#include <Core/ImGuiEngine.hpp>
#include <Resources/ResourceManager.hpp>
#include <ECS/Base/EntityManager.hpp>

namespace Core::Applications
{
	constexpr u32 DEFAULT_WIDTH = 1200;
	constexpr u32 DEFAULT_HEIGHT = 800;

	class TONIC_ENGINE_API Engine
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		static Window* p_s_window_;
		static Renderer::RHI* p_s_renderer_;

		static bool bImGuiInitialized_;
		static ImGuiEngine* p_s_imGui_;

		static Resources::ResourceManager* p_s_rManager_;
		static ECS::EntityManager* p_s_eManager_;

		static Engine* p_s_instance_;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		static Engine* CreateEngine();

		/* - Output : Returns Instance, or nullptr if Engine not initialised */
		static Engine* GetEngine();

		/* - Output : Returns Instance, or nullptr if Engine not initialised */
		static Renderer::RHI* GetRenderer();

		/* - Output : Returns Instance, or nullptr if Engine not initialised */
		static Window* GetWindow();

		/* - Output : Returns Instance, or nullptr if Engine not initialised */
		static Resources::ResourceManager* GetResourceManager();

		/* - Output : Returns Instance, or nullptr if Engine not initialised */
		static ECS::EntityManager* GetEntityManager();

		/* Output : Returns Instance, or nullptr if Engine not initialised OR Engine not initialised with _imGuiInit */
		static ImGuiEngine* GetImGui();

		static const float GetDeltaTime();

		/* Input : Name of application, width of window, height of window */
		const bool Init(const char* _appName, u32 _width, u32 _height, bool _imGuiInit = false);

		/* Called at the start of every frame */
		void StartFrame();
		/* Called to render every frame */
		void RunFrame();
		/* Called after rendering every frame */
		void EndFrame();

		/* Call to destroy Engine at end of program */
		void Destroy();

		/* Returns GLFW window should close */
		bool IsClosing();
	};
}

namespace // shortcuts
{
	using HEART = Core::Applications::Engine;

	// probably overkill

	// - Output : Returns Tonic Engine Instance, or nullptr if Tonic Engine not initialised
	auto GetENG() { return HEART::GetEngine(); };
	// - Output : Returns Tonic Engine Renderer Instance, or nullptr if Tonic Engine not initialised
	auto GetRDR() { return HEART::GetRenderer(); };
	// - Output : Returns Tonic Engine Window Instance, or nullptr if Tonic Engine not initialised
	auto GetWDW() { return HEART::GetRenderer(); };
	// - Output : Returns Tonic Engine Resource manager Instance, or nullptr if Tonic Engine not initialised
	auto GetRM() { return HEART::GetResourceManager(); };
	// - Output : Returns Tonic Engine Entity manager Instance, or nullptr if Tonic Engine not initialised
	auto GetEM() { return HEART::GetEntityManager(); };
	// - Output : Returns Tonic Engine ImGui Instance, or nullptr if Tonic Engine not initialised
	auto GetGUI() { return HEART::GetImGui(); };
}