#pragma once
#include "Core/Utils/Type.hpp"

#include "Core/Window.hpp"
#include "Core/RHI.hpp"
#include <Core/ImGuiEngine.hpp>

#include "Physics/PhysicsEngine.hpp"

#include <Resources/ResourceManager.hpp>
#include <ECS/Base/EntityManager.hpp>
#include "Core/InputManager.hpp"

#include "Resources/Sound.hpp"

struct ma_engine;

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
		static Window* s_p_window_;
		static Renderer::RHI* s_p_renderer_;

		ma_engine* s_p_audioEngine_ = nullptr;

		static Physics::PhysicsEngine* s_p_physicsEngine_;

		static bool bImGuiInitialized_;
		static ImGuiEngine* s_p_imGui_;

		static InputManager* s_p_inputManager_;

		static Resources::ResourceManager* s_p_rManager_;
		static ECS::EntityManager* s_p_eManager_;

		static Engine* s_p_instance_;

		float timeSinceLastFixedDeltaTime_ = 0.f;
		float fixedDeltaTime_ = 1 / 60.f;

	public:
		bool bIsPlaying = false;
		bool bIsStarting = false;
		bool bIsReset = false;

		LowRenderer::Cameras::FreeCamera* p_freeCamera = nullptr;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
		static Engine* CreateEngine();

		/* - Output : Returns Instance, or nullptr if Engine not initialised */
		static Engine* GetEngine();

		/* - Output : Returns Instance, or nullptr if Engine not initialised */
		static ma_engine* GetAudioEngine();

		/* - Output : Returns Instance, or nullptr if Engine not initialised */
		static Renderer::RHI* GetRenderer();

		/* - Output : Returns Instance, or nullptr if Engine not initialised */
		static Physics::PhysicsEngine* GetPhysicsEngine();

		/* - Output : Returns Instance, or nullptr if Engine not initialised */
		static Window* GetWindow();

		/* - Output : Returns Instance, or nullptr if Engine not initialised */
		static InputManager* GetInputManager();

		/* - Output : Returns Instance, or nullptr if Engine not initialised */
		static Resources::ResourceManager* GetResourceManager();

		/* - Output : Returns Instance, or nullptr if Engine not initialised */
		static ECS::EntityManager* GetEntityManager();

		/* Output : Returns Instance, or nullptr if Engine not initialised OR Engine not initialised with _imGuiInit */
		static ImGuiEngine* GetImGui();

		static const float GetDeltaTime();

		/* Input : Name of application, width of window, height of window */
		const bool Init(const char* _appName, u32 _width, u32 _height, bool _imGuiInit = false);

		// EDITOR'S FRAME
		// --------------

		/* Called to update every frame*/
		void UpdateScene();
		void StartGame();
		void UpdateGame();
		void LateUpdate();
		/* Called at the start of every frame */
		void StartFrame();
		/* Called to render every frame */
		void RunFrame(bool _scenemode = false);
		/* Called after rendering every frame */
		void EndFrame();

		/* Call to destroy Engine at end of program */
		void Destroy();

		/* Returns GLFW window should close */
		bool IsClosing();

		WindowBuffer* InitFBO();
		void BindFrame(WindowBuffer* _bufferObject);
		void UnBindFrame();

		const bool InitSystems();
	};
}

namespace // shortcuts
{
	struct
	{
		// - Output : Returns Tonic Engine Instance, or nullptr if Tonic Engine not initialised
		Core::Applications::Engine* GetEngineSelf() { return Core::Applications::Engine::GetEngine(); };
		// - Output : Returns Tonic Engine Renderer Instance, or nullptr if Tonic Engine not initialised
		Core::Renderer::RHI* GetRenderer() { return Core::Applications::Engine::GetRenderer(); };
		// - Output : Returns Tonic Engine Window Instance, or nullptr if Tonic Engine not initialised
		Core::Applications::Window* GetWindow() { return Core::Applications::Engine::GetWindow(); };
		// - Output : Returns MiniAudio Engine Instance, or nullptr if Tonic Engine not initialised
		ma_engine* GetAudioEngine() { return Core::Applications::Engine::GetAudioEngine(); };
		// - Output : Returns Tonic Engine Input manager Instance, or nullptr if Tonic Engine not initialised
		Core::Applications::InputManager* GetInputManager() { return Core::Applications::Engine::GetInputManager(); };
		// - Output : Returns Physics Engine Instance, or nullptr if Tonic Engine not initialised
		Physics::PhysicsEngine* GetPhysicsEngine() { return Core::Applications::Engine::GetPhysicsEngine(); };
		// - Output : Returns Tonic Engine Resource manager Instance, or nullptr if Tonic Engine not initialised
		Resources::ResourceManager* GetResourcesManager() { return Core::Applications::Engine::GetResourceManager(); };
		// - Output : Returns Tonic Engine Entity manager Instance, or nullptr if Tonic Engine not initialised
		ECS::EntityManager* GetEntitiesManager() { return Core::Applications::Engine::GetEntityManager(); };
		// - Output : Returns Tonic Engine ImGui Instance, or nullptr if Tonic Engine not initialised
		Core::Applications::ImGuiEngine* GetImGui() { return Core::Applications::Engine::GetImGui(); };

		__declspec(property(get = GetEngineSelf))
			Core::Applications::Engine* SELF;
		__declspec(property(get = GetRenderer))
			Core::Renderer::RHI* RDR;
		__declspec(property(get = GetWindow))
			Core::Applications::Window* WDW;
		__declspec(property(get = GetAudioEngine))
			ma_engine* AUD_ENG;
		__declspec(property(get = GetInputManager))
			Core::Applications::InputManager* INP_MNGR;
		__declspec(property(get = GetPhysicsEngine))
			Physics::PhysicsEngine* PHY_ENG;
		__declspec(property(get = GetResourcesManager))
			Resources::ResourceManager* RES_MNGR;
		__declspec(property(get = GetEntitiesManager))
			ECS::EntityManager* ENT_MNGR;
		__declspec(property(get = GetImGui))
			Core::Applications::ImGuiEngine* GUI;
	} ENGINE;
}