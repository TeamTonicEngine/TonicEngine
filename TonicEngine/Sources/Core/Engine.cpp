#include "pch.hpp"

#define MA_NO_VORBIS
#define MINIAUDIO_IMPLEMENTATION
#include "Audio/miniaudio.h"
#include "Audio/miniaudio_libvorbis.h"

#include "Core/Engine.hpp"
#include "Core/OpenGLWrapper.hpp"

#include "Resources/Resources.hpp"
#include <ECS/Systems.hpp>

Core::Applications::Engine* Core::Applications::Engine::p_s_instance_ = nullptr;
Core::Applications::Window* Core::Applications::Engine::p_s_window_ = nullptr;

bool Core::Applications::Engine::bImGuiInitialized_ = false;
Core::Applications::ImGuiEngine* Core::Applications::Engine::p_s_imGui_ = nullptr;

Core::Applications::InputManager* Core::Applications::Engine::p_s_inputManager_ = nullptr;

Resources::ResourceManager* Core::Applications::Engine::p_s_rManager_ = nullptr;
ECS::EntityManager* Core::Applications::Engine::p_s_eManager_ = nullptr;

Core::Renderer::RHI* Core::Applications::Engine::p_s_renderer_ = nullptr;

Core::Applications::Engine* Core::Applications::Engine::CreateEngine()
{
	if (!p_s_instance_)
		p_s_instance_ = new Engine();

	return p_s_instance_;
}

Core::Applications::Engine* Core::Applications::Engine::GetEngine() { return p_s_instance_; }

ma_engine* Core::Applications::Engine::GetAudioEngine() { return p_s_instance_->p_s_audioEngine_; }

Core::Renderer::RHI* Core::Applications::Engine::GetRenderer() { return p_s_renderer_; }

Core::Applications::Window* Core::Applications::Engine::GetWindow() { return p_s_window_; }

Core::Applications::InputManager* Core::Applications::Engine::GetInputManager() { return p_s_inputManager_; }

Resources::ResourceManager* Core::Applications::Engine::GetResourceManager() { return p_s_rManager_; }

ECS::EntityManager* Core::Applications::Engine::GetEntityManager() { return p_s_eManager_; }

Core::Applications::ImGuiEngine* Core::Applications::Engine::GetImGui() { return p_s_imGui_; }

const float Core::Applications::Engine::GetDeltaTime() { return p_s_window_->GetDeltaTime(); }

const bool Core::Applications::Engine::Init(const char* _appName, u32 _width, u32 _height, bool _imGuiInit)
{
	static bool s_bInit = false;
	if (s_bInit)
	{
		DEBUG_ERROR("Engine is already initialised /!\\");
		return p_s_window_;
	}

	// Init the window
	p_s_window_ = new Window(_appName, _width, _height);
	bool bWindowInit = p_s_window_->Init();

	// Init the renderer
	p_s_renderer_ = new Renderer::OpenGLWrapper();
	bool bRendererInit = p_s_renderer_->Init(p_s_window_);

	//Init the Audio engine
	p_s_audioEngine_ = new ma_engine();
	//Init()
	ma_result result = ma_engine_init(NULL, p_s_audioEngine_);
	bool bAudioInit = result == MA_SUCCESS;
	if (!bAudioInit)
		DEBUG_ERROR("MINI AUDIO INIT FAIL")
	else
		DEBUG_SUCCESS("MINI AUDIO INIT SUCCESSFULLY");

	// ImGUI
		// Init Imgui if demandeD
	if (_imGuiInit)
		p_s_imGui_ = new ImGuiEngine(p_s_window_);
	bImGuiInitialized_ = _imGuiInit;

	p_s_inputManager_ = new Core::Applications::InputManager();
	p_s_inputManager_->Init();

	// Init the resource manager
	p_s_rManager_ = new Resources::ResourceManager();
	bool bRManagerInit = p_s_rManager_->Init();

	p_s_eManager_ = new ECS::EntityManager();

	// Init default shader
	using namespace Resources;
	auto defaultShader = p_s_rManager_->Create<Shader>("Assets\\Shaders\\phong.frag");
	auto unlitShader = p_s_rManager_->Create<Shader>("Assets\\Shaders\\unlit.frag");
	auto basicShader = p_s_rManager_->Create<Shader>("Assets\\Shaders\\basic.frag");
	auto pbrShader = p_s_rManager_->Create<Shader>("Assets\\Shaders\\pbr.frag"); // TODELETE
	auto whiteShader = p_s_rManager_->Create<Shader>("Assets\\Shaders\\white.frag"); // Basic all white shader
	auto skyboxShader = p_s_rManager_->Create<Resources::Shader>("Assets\\Shaders\\skybox.frag");

	defaultShader->Use();

	// Init Sound
	auto soundClap = p_s_rManager_->Create<Sound>("Assets\\Sounds\\clap.wav"); // WAV
	soundClap->LoadSound();

	auto soundShip = p_s_rManager_->Create<Sound>("Assets\\Sounds\\ship.wav"); // WAV
	soundShip->LoadSound();

	auto soundWaves = p_s_rManager_->Create<Sound>("Assets\\Sounds\\waves.wav"); // WAV
	soundWaves->LoadSound();

	auto soundBurp = p_s_rManager_->Create<Sound>("Assets\\Sounds\\burp.ogg"); // OGG
	soundBurp->LoadSound();

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
	unsigned int cubemapTexture = p_s_renderer_->LoadCubemap(faces);

	auto white = p_s_rManager_->Create<Texture>("StaticAssets\\Textures\\white.png");

	const bool bSystemsInit = InitSystems();
	s_bInit = bWindowInit && bRendererInit && bAudioInit && bRManagerInit && defaultShader && bSystemsInit;
	return s_bInit;
}

const bool Core::Applications::Engine::InitSystems()
{
	bool success = true;
	//HYPER IMPORTANT TO SETUP THE TRANSFORMSYSTEM 1st
	success &= p_s_eManager_->RegisterSystem<ECS::Systems::TransformSystem>();
	success &= p_s_eManager_->RegisterSystem<ECS::Systems::CameraSystem>();
	success &= p_s_eManager_->RegisterSystem<ECS::Systems::MeshRendererSystem>();
	success &= p_s_eManager_->RegisterSystem<ECS::Systems::PointLightSystem>();
	success &= p_s_eManager_->RegisterSystem<ECS::Systems::SpotLightSystem>();
	success &= p_s_eManager_->RegisterSystem<ECS::Systems::DirectionalLightSystem>();
	success &= p_s_eManager_->RegisterSystem<ECS::Systems::AudioSourceSystem>();
	success &= p_s_eManager_->RegisterSystem<ECS::Systems::AudioListenerSystem>();
	success &= p_s_eManager_->RegisterSystem<ECS::Systems::CollisionDetectionSystem>();

	//success &= p_s_eManager_->Init();
	return success;
}

void Core::Applications::Engine::StartFrame()
{
	p_s_window_->StartFrame();

	if (bImGuiInitialized_)
		p_s_imGui_->StartFrame();

	p_s_inputManager_->StartHoldEvents();
	p_s_inputManager_->ProcessEvent();
	// physic process ?
}

WindowBuffer* Core::Applications::Engine::InitFBO()
{
	return p_s_renderer_->InitFrameBuffer(); // InitFramebuffer needs a shader to exist and be active
}

void Core::Applications::Engine::BindFrame(WindowBuffer* _bufferObject)
{
	p_s_renderer_->BindFrameBuffer(_bufferObject);
	p_s_renderer_->StartFrame();
}

void Core::Applications::Engine::UnBindFrame()
{
	p_s_renderer_->UnbindFrameBuffer();
}

void Core::Applications::Engine::UpdateScene()
{
	p_s_renderer_->UpdateCurrentCamera(&p_s_window_->GetFrameInput()->cameraInput);
	p_s_window_->ProcessInput();

	p_s_eManager_->Update();
	//TODO: Add a Fixed Update and call the 3 updates independently
	p_s_eManager_->LateUpdate();
}

void Core::Applications::Engine::UpdateGame()
{
	//p_s_renderer_.up
	//p_s_renderer_->UpdateCurrentCamera(&p_s_window_->GetFrameInput()->cameraInput);

	//Add an p_s_eManager_ in Game Update here

	//p_s_eManager_->Update();

	//p_s_eManager_->LateUpdate();
}
void Core::Applications::Engine::LateUpdate()
{
	p_s_eManager_->LateUpdate();
}
void Core::Applications::Engine::RunFrame(bool _scenemode)
{
	if (_scenemode)
		p_s_eManager_->RenderEditorScene();
	else
		p_s_eManager_->Render();
	if (p_s_window_->IsFramebufferResized())
	{
		p_s_renderer_->FrameBufferResized();

		p_s_window_->ResizedHandled();
	}
}

void Core::Applications::Engine::EndFrame()
{
	if (bImGuiInitialized_)
		p_s_imGui_->EndFrame();

	p_s_window_->EndFrame();
	p_s_renderer_->EndFrame();
}

void Core::Applications::Engine::Destroy()
{
	if (bImGuiInitialized_)
		delete p_s_imGui_;

	p_s_eManager_->Destroy();

	delete p_s_eManager_;
	delete p_s_rManager_;

	delete p_s_inputManager_;

	ma_engine_uninit(p_s_audioEngine_);
	delete p_s_audioEngine_;

	p_s_renderer_->Destroy();
	p_s_window_->Destroy();

	delete p_s_renderer_;
	delete p_s_window_;

	Debug::Log::DeleteInstance();
}

bool Core::Applications::Engine::IsClosing() { return p_s_window_->IsClosing(); }