#include "pch.hpp"

#include "Core/Engine.hpp"

#include "Core/OpenGLWrapper.hpp"

Core::Applications::Engine* Core::Applications::Engine::p_s_instance_ = nullptr;
Core::Applications::Window* Core::Applications::Engine::p_s_window_ = nullptr;

bool Core::Applications::Engine::bImGuiInitialized_ = false;
Core::Applications::ImGuiEngine* Core::Applications::Engine::p_s_imGui_ = nullptr;

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

Core::Renderer::RHI* Core::Applications::Engine::GetRenderer() { return p_s_renderer_; }

Core::Applications::Window* Core::Applications::Engine::GetWindow() { return p_s_window_; }

Resources::ResourceManager* Core::Applications::Engine::GetResourceManager() { return p_s_rManager_; }

ECS::EntityManager* Core::Applications::Engine::GetEntityManager() { return p_s_eManager_; }

Core::Applications::ImGuiEngine* Core::Applications::Engine::GetImGui() { return p_s_imGui_; }

const float Core::Applications::Engine::GetDeltaTime() { return p_s_window_->GetDeltaTime(); }

#include "Resources/Shader.hpp"
using Resources::Shader;
#include "Resources/Mesh.hpp"
using Resources::Mesh;
#include "Resources/Texture.hpp"
using Resources::Texture;
#include "Resources/Sound.hpp"
using Resources::Sound;
const bool Core::Applications::Engine::Init(const char* _appName, u32 _width, u32 _height, bool _imGuiInit)
{
	static bool bInit = false;
	if (bInit)
	{
		DEBUG_ERROR("Engine is already initialised /!\\");
		return p_s_window_;
	}

	// Init the window
	p_s_window_ = new Window(_appName, _width, _height);
	bool windowInit = p_s_window_->Init();

	// Init the renderer
	p_s_renderer_ = new Renderer::OpenGLWrapper();
	bool rendererInit = p_s_renderer_->Init(p_s_window_);

	// Init Imgui if demanded
	if (_imGuiInit)
		p_s_imGui_ = new ImGuiEngine(p_s_window_);
	bImGuiInitialized_ = _imGuiInit;

	// Init the resource manager
	p_s_rManager_ = new Resources::ResourceManager();
	bool rManagerInit = p_s_rManager_->Init();

	p_s_eManager_ = new ECS::EntityManager();

	// Init default shader
	auto defaultShader = p_s_rManager_->Create<Shader>("lighting_test");
	p_s_rManager_->Create<Shader>("basic");
	p_s_rManager_->Create<Shader>("white"); // Basic all white shader

	defaultShader->Use();
	p_s_renderer_->InitFrameBuffer(); // InitFramebuffer needs a shader to exist and be active

	p_s_rManager_->Create<Mesh>("backpack"); // TODELETE
	p_s_rManager_->Create<Mesh>("sphere"); // TODELETE
	auto textSpe = p_s_rManager_->Create<Texture>("backpack_specular"); // TODELETE
	textSpe->SetTextureType(Resources::TextureType::Specular);
	auto textNor = p_s_rManager_->Create<Texture>("backpack_normal"); // TODELETE
	textNor->SetTextureType(Resources::TextureType::Normal);
	auto textDif = p_s_rManager_->Create<Texture>("backpack_diffuse"); // TODELETE
	textDif->SetTextureType(Resources::TextureType::Diffuse);

	// Init Sound

	auto soundClap = p_s_rManager_->Create<Sound>("Applaudissements");
	soundClap->LoadSound();
	soundClap->Play();

	bInit = windowInit && rendererInit && rManagerInit && defaultShader;
	return bInit;
}

void Core::Applications::Engine::StartFrame()
{
	p_s_window_->StartFrame();
	p_s_window_->ProcessInput();

	p_s_renderer_->BindFrameBuffer();
	p_s_renderer_->StartFrame();

	if (bImGuiInitialized_)
		p_s_imGui_->StartFrame();

	// physic process ?
}

void Core::Applications::Engine::RunFrame()
{
	p_s_renderer_->UpdateCurrentCamera(&p_s_window_->GetFrameInput()->cameraInput);

	if (p_s_window_->IsFramebufferResized())
	{
		p_s_renderer_->FrameBufferResized();

		p_s_renderer_->Draw();

		p_s_window_->ResizedHandled();
	}
	else
	{
		p_s_renderer_->Draw();
	}
}

void Core::Applications::Engine::EndFrame()
{
	p_s_renderer_->UnbindFrameBuffer();

	if (bImGuiInitialized_)
		p_s_imGui_->EndFrame();

	p_s_window_->EndFrame();
	p_s_renderer_->EndFrame();
}

void Core::Applications::Engine::Destroy()
{
	if (bImGuiInitialized_)
		delete p_s_imGui_;

	delete p_s_rManager_;
	delete p_s_eManager_;

	p_s_renderer_->Destroy();
	p_s_window_->Destroy();
}

bool Core::Applications::Engine::IsClosing() { return p_s_window_->IsClosing(); }