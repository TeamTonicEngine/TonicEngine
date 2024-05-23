#include "pch.hpp"

#define MA_NO_VORBIS
#define MINIAUDIO_IMPLEMENTATION
#include "Audio/miniaudio.h"
#include "Audio/miniaudio_libvorbis.h"

#include "Core/Engine.hpp"
#include "Core/OpenGLWrapper.hpp"

#include "Physics/JoltWrapper.hpp"

#include "Resources/Resources.hpp"

#include <ECS/Systems.hpp>
#include <ECS/Components/CppScriptComponent.hpp>

Core::Applications::Engine* Core::Applications::Engine::s_p_instance_ = nullptr;
Core::Applications::Window* Core::Applications::Engine::s_p_window_ = nullptr;

bool Core::Applications::Engine::bImGuiInitialized_ = false;
Core::Applications::ImGuiEngine* Core::Applications::Engine::s_p_imGui_ = nullptr;

Core::Applications::InputManager* Core::Applications::Engine::s_p_inputManager_ = nullptr;

Resources::ResourceManager* Core::Applications::Engine::s_p_rManager_ = nullptr;
ECS::EntityManager* Core::Applications::Engine::s_p_eManager_ = nullptr;

Core::Renderer::RHI* Core::Applications::Engine::s_p_renderer_ = nullptr;
Physics::PhysicsEngine* Core::Applications::Engine::s_p_physicsEngine_ = nullptr;

Core::Applications::Engine* Core::Applications::Engine::CreateEngine()
{
	if (!s_p_instance_)
		s_p_instance_ = new Engine();

	return s_p_instance_;
}

Core::Applications::Engine* Core::Applications::Engine::GetEngine() { return s_p_instance_; }

ma_engine* Core::Applications::Engine::GetAudioEngine() { return GetEngine()->s_p_audioEngine_; }

Core::Renderer::RHI* Core::Applications::Engine::GetRenderer() { return s_p_renderer_; }

Core::Applications::Window* Core::Applications::Engine::GetWindow() { return s_p_window_; }

Core::Applications::InputManager* Core::Applications::Engine::GetInputManager() { return s_p_inputManager_; }

Resources::ResourceManager* Core::Applications::Engine::GetResourceManager() { return s_p_rManager_; }

ECS::EntityManager* Core::Applications::Engine::GetEntityManager() { return s_p_eManager_; }

Physics::PhysicsEngine* Core::Applications::Engine::GetPhysicsEngine() { return s_p_physicsEngine_; }

Core::Applications::ImGuiEngine* Core::Applications::Engine::GetImGui() { return s_p_imGui_; }

const float Core::Applications::Engine::GetDeltaTime() { return s_p_window_->GetDeltaTime(); }

const bool Core::Applications::Engine::Init(const char* _appName, u32 _width, u32 _height, bool _imGuiInit)
{
	static bool s_bInit = false;
	if (s_bInit)
	{
		DEBUG_ERROR("Engine is already initialized /!\\");
		return s_p_window_;
	}

	// Init the window
	s_p_window_ = new Window(_appName, _width, _height);
	bool bWindowInit = s_p_window_->Init();

	// Init the renderer
	s_p_renderer_ = new Renderer::OpenGLWrapper();
	bool bRendererInit = s_p_renderer_->Init(s_p_window_);

	// Init Physics Engine
	s_p_physicsEngine_ = new Physics::JoltWrapper();
	bool bPhysicsInit = s_p_physicsEngine_->Init();

	// Init Audio Engine
	s_p_audioEngine_ = new ma_engine();
	ma_result result = ma_engine_init(NULL, s_p_audioEngine_);
	bool bAudioInit = result == MA_SUCCESS;
	if (!bAudioInit)
		DEBUG_ERROR("MINI AUDIO INIT FAIL")
	else
		DEBUG_SUCCESS("MINI AUDIO INIT SUCCESSFULLY");

	// ImGUI
		// Init Imgui if demanded
	if (_imGuiInit)
		s_p_imGui_ = new ImGuiEngine(s_p_window_);
	bImGuiInitialized_ = _imGuiInit;

	s_p_inputManager_ = new Core::Applications::InputManager();
	s_p_inputManager_->Init();

	s_p_window_->RegisterInputHandler();

	// Init the resource manager
	s_p_rManager_ = new Resources::ResourceManager();
	bool bRManagerInit = s_p_rManager_->Init();

	s_p_eManager_ = new ECS::EntityManager();

	// Init default shader
	using namespace Resources;
	auto defaultShader = s_p_rManager_->Create<Shader>("Assets\\Shaders\\phong.frag");
	auto unlitShader = s_p_rManager_->Create<Shader>("Assets\\Shaders\\unlit.frag");
	auto basicShader = s_p_rManager_->Create<Shader>("Assets\\Shaders\\basic.frag");
	auto pbrShader = s_p_rManager_->Create<Shader>("Assets\\Shaders\\pbr.frag"); // TODELETE
	auto whiteShader = s_p_rManager_->Create<Shader>("Assets\\Shaders\\white.frag"); // Basic all white shader
	auto skyboxShader = s_p_rManager_->Create<Resources::Shader>("Assets\\Shaders\\skybox.frag");
	Resources::Font::InitShader(s_p_rManager_->Create<Resources::Shader>("Assets\\Shaders\\text.frag"));

	defaultShader->Use();

	// Init Sound
	auto soundClap = s_p_rManager_->Create<Sound>("Assets\\Sounds\\clap.wav"); // WAV
	soundClap->LoadSound();

	auto soundBurp = s_p_rManager_->Create<Sound>("Assets\\Sounds\\burp.ogg"); // OGG
	soundBurp->LoadSound();

	auto soundShip = s_p_rManager_->Create<Sound>("Assets\\Sounds\\ship.wav"); // OGG
	soundShip->LoadSound();

	auto soundWaves = s_p_rManager_->Create<Sound>("Assets\\Sounds\\waves.wav"); // OGG
	soundWaves->LoadSound();
	

	//Skybox
	vector<std::string> faces
	{
		"StaticAssets\\Skybox\\right.jpg",
		"StaticAssets\\Skybox\\left.jpg",
		"StaticAssets\\Skybox\\top.jpg",
		"StaticAssets\\Skybox\\bottom.jpg",
		"StaticAssets\\Skybox\\front.jpg",
		"StaticAssets\\Skybox\\back.jpg"
	};
	unsigned int cubemapTexture = s_p_renderer_->LoadCubemap(faces);

	p_freeCamera = new LowRenderer::Cameras::FreeCamera(_width, _height);
	s_p_renderer_->SetCamera(p_freeCamera);
	p_freeCamera->bUsed = true;

	auto white = s_p_rManager_->Create<Texture>("StaticAssets\\Textures\\white.png");

	const bool bSystemsInit = InitSystems();
	s_bInit = bWindowInit && bRendererInit && bPhysicsInit && bAudioInit && bRManagerInit && defaultShader && bSystemsInit;
	return s_bInit;
}

const bool Core::Applications::Engine::InitSystems()
{
	bool success = true;
	//HYPER IMPORTANT TO SETUP THE TRANSFORMSYSTEM 1st
	success &= s_p_eManager_->RegisterSystem<ECS::Systems::TransformSystem>();
	success &= s_p_eManager_->RegisterSystem<ECS::Systems::CameraSystem>();
	success &= s_p_eManager_->RegisterSystem<ECS::Systems::MeshRendererSystem>();
	success &= s_p_eManager_->RegisterSystem<ECS::Systems::PointLightSystem>();
	success &= s_p_eManager_->RegisterSystem<ECS::Systems::SpotLightSystem>();
	success &= s_p_eManager_->RegisterSystem<ECS::Systems::DirectionalLightSystem>();
	success &= s_p_eManager_->RegisterSystem<ECS::Systems::AudioSourceSystem>();
	success &= s_p_eManager_->RegisterSystem<ECS::Systems::AudioListenerSystem>();
	success &= s_p_eManager_->RegisterSystem<ECS::Systems::BoxRigidbodySystem>();
	success &= s_p_eManager_->RegisterSystem<ECS::Systems::SphereRigidbodySystem>();
	success &= s_p_eManager_->RegisterSystem<ECS::Systems::CapsuleRigidbodySystem>();
	success &= s_p_eManager_->RegisterSystem<ECS::Systems::TextRendererSystem>();

	//success &= s_p_eManager_->Init();
	return success;
}

void Core::Applications::Engine::StartFrame()
{
	s_p_window_->StartFrame();

	if (bImGuiInitialized_)
		s_p_imGui_->StartFrame();

	s_p_inputManager_->StartHoldEvents();
	s_p_inputManager_->ProcessEvent();
	// physic process ?
}

WindowBuffer* Core::Applications::Engine::InitFBO()
{
	return s_p_renderer_->InitFrameBuffer(); // InitFramebuffer needs a shader to exist and be active
}

void Core::Applications::Engine::BindFrame(WindowBuffer* _bufferObject)
{
	s_p_renderer_->BindFrameBuffer(_bufferObject);
	s_p_renderer_->StartFrame();
}

void Core::Applications::Engine::UnBindFrame()
{
	s_p_renderer_->UnbindFrameBuffer();
}

void Core::Applications::Engine::UpdateScene()
{
	// ECS
	s_p_eManager_->Update();
	//TODO: Add a Fixed Update and call the 3 updates independently
	s_p_eManager_->LateUpdate();
}

void Core::Applications::Engine::StartGame()
{
	auto& scriptList = s_p_eManager_->GetCompList<ECS::Components::CppScriptComponent>()->data_;
	for (auto& script : scriptList)
	{
		if (!script.p_instance)
		{
			script.p_instance = script.InstantiateScript();
			script.p_instance->OnCreate();
		}
		script.p_instance->OnStart();
	}
}

void Core::Applications::Engine::UpdateGame()
{
	timeSinceLastFixedDeltaTime_ += GetDeltaTime();
	if (timeSinceLastFixedDeltaTime_ >= fixedDeltaTime_)
		s_p_physicsEngine_->Update(timeSinceLastFixedDeltaTime_);

	auto& scriptList = s_p_eManager_->GetCompList<ECS::Components::CppScriptComponent>()->data_;
	for (auto& script : scriptList)
	{
		if (!script.p_instance)
		{
			script.p_instance = script.InstantiateScript();
			script.p_instance->OnCreate();
		}
		script.p_instance->OnUpdate(GetDeltaTime());
	}

	while (timeSinceLastFixedDeltaTime_ >= fixedDeltaTime_)
	{
		timeSinceLastFixedDeltaTime_ -= fixedDeltaTime_;
		for (auto& script : scriptList)
			script.p_instance->OnFixedUpdate(fixedDeltaTime_);
	}

	//s_p_renderer_.up
	//s_p_renderer_->UpdateCurrentCamera(&s_p_window_->GetFrameInput()->cameraInput);

	//Add an s_p_eManager_ in Game Update here

	//s_p_eManager_->Update();

	//s_p_eManager_->LateUpdate();
}

void Core::Applications::Engine::LateUpdate()
{
	s_p_eManager_->LateUpdate();
}

void Core::Applications::Engine::RunFrame(bool _scenemode)
{
	if (_scenemode)
		s_p_eManager_->RenderEditorScene();
	else
		s_p_eManager_->Render();

	if (s_p_window_->IsFramebufferResized())
	{
		s_p_renderer_->FrameBufferResized();
		s_p_window_->ResizedHandled();
	}
}

void Core::Applications::Engine::EndFrame()
{
	s_p_inputManager_->resetScrollOffset();

	if (bImGuiInitialized_)
		s_p_imGui_->EndFrame();

	s_p_window_->EndFrame();
	s_p_renderer_->EndFrame();
}

void Core::Applications::Engine::Destroy()
{
	delete p_freeCamera;

	if (bImGuiInitialized_)
		delete s_p_imGui_;

	s_p_eManager_->Destroy();

	delete s_p_eManager_;
	delete s_p_rManager_;

	delete s_p_inputManager_;

	s_p_physicsEngine_->Destroy();
	delete s_p_physicsEngine_;

	ma_engine_uninit(s_p_audioEngine_);
	delete s_p_audioEngine_;

	s_p_renderer_->Destroy();
	s_p_window_->Destroy();

	delete s_p_renderer_;
	delete s_p_window_;

	Debug::Log::DeleteInstance();
}

bool Core::Applications::Engine::IsClosing() { return s_p_window_->IsClosing(); }