#include "Core/AppEditor.hpp"

#include <iostream>

#include <Resources/Physics.hpp>
#include <LowRenderer/Cameras/FreeCamera.hpp>

Core::Applications::AppEditor::AppEditor(std::string _appName, u32 _width, u32 _height)
{
	p_engine_ = Engine::CreateEngine();

	if (p_engine_->Init(_appName.c_str(), _width, _height, true))
		DEBUG_SUCCESS("INITIALIZED ENGINE")
	else
		throw ("FAILED TO INITIALIZE ENGINE");

	p_sceneFBO = p_engine_->InitFBO();
	p_gameFBO = p_engine_->InitFBO();

	Init();

	p_editorWindow_ = new EditorWindow(ENGINE.GUI);
	if (p_editorWindow_)
		DEBUG_SUCCESS("INITIALIZED EDITOR WINDOW")
	else
		throw ("FAILED TO INITIALIZE EDITOR WINDOW");

	p_freeCamera_ = new LowRenderer::Cameras::FreeCamera(_width, _height);

	//p_GameCamera_ = new LowRenderer::Cameras::Camera(_width, _height);
	ENGINE.RDR->SetCamera(p_freeCamera_);
	//ENGINE.RDR->SetCamera(p_GameCamera_, Maths::Vec3(0.f, 0.f, -10.f));
	//p_GameCamera_->bUsed = true;
	p_freeCamera_->bUsed = true;
}

Core::Applications::AppEditor::~AppEditor()
{
	delete p_freeCamera_;
	delete p_sceneFBO;
	delete p_gameFBO;
	delete p_editorWindow_;
	// End of app
	p_engine_->Destroy();
	delete p_engine_;
};

void Core::Applications::AppEditor::Init()
{
	//Load Editor Resources No multi thread
	auto p_RM = ENGINE.RES_MNGR;									//Reload //Multithread
	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\folder.png", false, false);
	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\file.png", false, false);
	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\image-file.png", false, false);
	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\favourite-file.png", false, false);
	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\audio-file.png", false, false);
	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\3d-file.png", false, false);
	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\scene-file.png", false, false);

	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\play.png", false, false);
	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\pause.png", false, false);
	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\stop.png", false, false);
	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\trash.png", false, false);
	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\plus.png", false, false);
	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\lock.png", false, false);
	p_RM->Create<Resources::Texture>("StaticAssets\\Images\\unlock.png", false, false);
	///////////////////////////////////////////////////////////////////////////////////////////
//Should go in Engine

	p_RM->Create<Resources::Texture>("StaticAssets\\Textures\\white25pc.png");
	std::vector<Resources::TexturePtr> textures = {
	p_RM->Create<Resources::Texture>("StaticAssets\\Textures\\white.png"),
	p_RM->Create<Resources::Texture>("StaticAssets\\Textures\\magenta.png"),
	p_RM->Create<Resources::Texture>("StaticAssets\\Textures\\Debugempty.png")
	};
	std::vector<Resources::MeshPtr> meshes = {
	p_RM->Create<Resources::Mesh>("StaticAssets\\Meshes\\sphere.obj"),
	p_RM->Create<Resources::Mesh>("StaticAssets\\Meshes\\cube.obj"),
	p_RM->Create<Resources::Mesh>("StaticAssets\\Meshes\\cylinder.obj"),
	p_RM->Create<Resources::Mesh>("StaticAssets\\Meshes\\capsule.obj"),
	p_RM->Create<Resources::Mesh>("StaticAssets\\Meshes\\cone.obj"),
	};
}

#include <ECS/Components.hpp> //TODO Test ECS TODELETE
void Core::Applications::AppEditor::Run()
{
	//Physics::Physics p;
	//p.Test();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//Test ECS TODELETE
	ECS::EntityManager* p_em = ENGINE.ENT_MNGR;

	//Empty entity(Not the root)
	ECS::EntityID e0 = p_em->AddNewEntity();

	//PointLight1
	ECS::EntityID e1 = p_em->AddNewEntity();
	ECS::Components::TransformComponent& t = p_em->AddComponent<ECS::Components::TransformComponent>(e1);
	ECS::Components::PointLightComponent& pl = p_em->AddComponent<ECS::Components::PointLightComponent>(e1);
	ECS::Components::MeshRendererComponent* m1 = &p_em->AddComponent<ECS::Components::MeshRendererComponent>(e1, "StaticAssets\\Meshes\\sphere.obj");
	auto white = ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Textures\\white.png");
	white->textureType = Resources::TextureType::Diffuse;
	m1->renderer.p_materials.push_back(std::make_shared<Resources::Material>(white, white));
	t.position = Maths::Vec3(0.f, 0.f, 10.f);

	//DirectionalLight1
	ECS::EntityID e2 = p_em->AddNewEntity();
	ECS::Components::TransformComponent* t2 = &p_em->AddComponent<ECS::Components::TransformComponent>(e2);
	p_em->AddComponent<ECS::Components::DirectionalLightComponent>(e2);
	t2->position = Maths::Vec3(0.f, -1.f, 0.f);

	//SpotLight1
	ECS::EntityID e3 = p_em->AddNewEntity();
	ECS::Components::TransformComponent* t3 = &p_em->AddComponent<ECS::Components::TransformComponent>(e3);
	ECS::Components::SpotLightComponent& spl = p_em->AddComponent<ECS::Components::SpotLightComponent>(e3);
	t3->position = Maths::Vec3(0.f, 0.f, -10.f);
	//t3->rotation = Maths::Quaternions::FromEulerAngles({ 0.f, 180.f * Maths::Constants::DEG2RAD, 0.f });

	//Camera1
	ECS::EntityID e4 = p_em->AddNewEntity();
	ECS::Components::TransformComponent& t4 = p_em->AddComponent<ECS::Components::TransformComponent>(e4);
	ECS::Components::CameraComponent& c1 = p_em->AddComponent<ECS::Components::CameraComponent>(e4);
	c1.StopUseCamera();
	t4.position = Maths::Vec3(0.f, 0.f, -10.f);

	//Mesh1
	ECS::EntityID e5 = p_em->AddNewEntity();
	ECS::Components::TransformComponent* t5 = &p_em->AddComponent<ECS::Components::TransformComponent>(e5);
	auto bakupakumeshu = ENGINE.RES_MNGR->Create<Resources::Mesh>("Assets\\Meshes\\backpack.obj");
	ECS::Components::MeshRendererComponent& m2 = p_em->AddComponent<ECS::Components::MeshRendererComponent>(e5, bakupakumeshu);
	auto diffuse = ENGINE.RES_MNGR->Create<Resources::Texture>("Assets\\Textures\\Back\\backpack_diffuse.jpg");
	diffuse->textureType = Resources::TextureType::Diffuse;

	auto specular = ENGINE.RES_MNGR->Create<Resources::Texture>("Assets\\Textures\\Back\\backpack_specular.jpg");
	specular->textureType = Resources::TextureType::Specular;

	auto normal = ENGINE.RES_MNGR->Create<Resources::Texture>("Assets\\Textures\\Back\\backpack_normal.png");
	normal->textureType = Resources::TextureType::Normal;
	auto roughness = ENGINE.RES_MNGR->Create<Resources::Texture>("Assets\\Textures\\Back\\backpack_roughness.jpg");
	roughness->textureType = Resources::TextureType::Roughness;
	auto ao = ENGINE.RES_MNGR->Create<Resources::Texture>("Assets\\Textures\\Back\\backpack_ao.jpg");
	ao->textureType = Resources::TextureType::AO;
	m2.renderer.p_materials.push_back(std::make_shared<Resources::Material>(diffuse, specular, ao, normal, roughness));
	t5->position = Maths::Vec3(0.f, 0.f, -20.f);
	p_em->AddChild(e0, e3);
	p_em->AddChild(e3, e5);

	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Keyboard::KeyW, []() {std::cout << "W Event\n"; });
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Keyboard::KeyA, []() {std::cout << "A Event\n"; });
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Keyboard::KeyS, []() {std::cout << "S Event\n"; });
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Keyboard::KeyD, []() {std::cout << "D Event\n"; });
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Mouse::ButtonLeft, []() {std::cout << "Mouse Left Event\n"; });
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Mouse::ButtonRight, []() {std::cout << "Mouse Right Event\n"; });
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Mouse::ButtonMiddle, []() {std::cout << "Mouse Middle Event\n"; });

	//ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::MoveMouse, []() {std::cout << "Move Mouse Event\n"; });

	//HERE For Performance tests
	/*
	for (size_t i = 7; i < ECS::MAX_ENTITY_COUNT; i++)
	{
		auto e = p_em->AddNewEntity();
		p_em->AddComponent<ECS::Components::TransformComponent>(e);
		if (i % 3)
		{
			p_em->ParentEntity(i, i - 1);
		}
	}
	*/

	////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Main Loop
	static float angle = 180.f * Maths::Constants::DEG2RAD;
	while (!p_engine_->IsClosing())
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	//Test ECS TODELETE
		if (p_editorWindow_->bIsPlaying_)
		{
			t3 = &p_em->GetComponent<ECS::Components::TransformComponent>(e3);
			angle += .005f;
			t3->rotation = Maths::Quaternions::FromEulerAngles({ 0.f, angle,0.f });
			t3->position += Maths::Vec3(0.f, 0.1f, 0.f);

			t5 = &p_em->GetComponent<ECS::Components::TransformComponent>(e5);
			//t5->position.x += 0.1f;
			t5->rotation = Maths::Quaternions::FromEulerAngles({ angle, 0.f,0.f });
		}
		///////////////////////////////////////////////////////////////////////////////
		//UPDATE
		p_engine_->UpdateScene();

		// START
		p_engine_->StartFrame();
		p_editorWindow_->SetUpWindows();

		/*u32_2 size = ENGINE.WDW->GetWindowSize();
		ENGINE.RDR->ResizeViewPort(size.x, size.y);
		p_freeCamera_->aspect = static_cast<float>(size.x) / size.y;
		p_freeCamera_->SetProjection();
		p_freeCamera_->bProjChanged = true;
		p_engine_->RunFrame();*/

		PlayScene();
		PlayGame();

		// END
		p_engine_->EndFrame();
	}
}

void Core::Applications::AppEditor::PlayScene()
{
	p_engine_->BindFrame(p_sceneFBO);
	{
		// CREATE THE SCENE WINDOW
		// -----------------------
		p_editorWindow_->StartWindow("Scene");

		//static ImVec2 preSize = ImGui::GetWindowSize();
		ImVec2 size = ImGui::GetWindowSize();

		//ENGINE.RDR

		p_freeCamera_->aspect = static_cast<float>(size.x) / size.y;
		p_freeCamera_->SetProjection();
		p_freeCamera_->bProjChanged = true;

		// CURRENT WINDOW
		// --------------
		if (p_editorWindow_->bIsSceneOpen_)
		{
			// TODO : Don't forget to reuse the freeCamera if you turn off it  (exemple of turning of : p_freeCamera_->bUsed = false)

			// UPDATE
			// ------
			//p_engine_->UpdateScene();
		}

		// RENDERING
		// ---------
		p_engine_->RunFrame(true);
		p_editorWindow_->DrawWindow(p_sceneFBO->ID);
	}
	p_engine_->UnBindFrame();
}

void Core::Applications::AppEditor::PlayGame()
{
	if (p_editorWindow_->gameWindow)
	{
		p_engine_->BindFrame(p_gameFBO);
		// CREATE THE GAME WINDOW
		// ----------------------
		p_editorWindow_->StartWindow("Game", &p_editorWindow_->gameWindow);

		//static ImVec2 preSize = ImGui::GetWindowSize();
		ImVec2 size = ImGui::GetWindowSize();
		p_freeCamera_->aspect = static_cast<float>(size.x) / size.y;
		p_freeCamera_->SetProjection();
		p_freeCamera_->bProjChanged = true;

		// CURRENT WINDOW
		// --------------
		if (p_editorWindow_->bIsGameOpen_)
		{
			// TODO: Use default Game Camera

			//c1.UseCamera();
			//p_freeCamera_->bUsed = false;
		}

		// PLAY
		// ----
		if (p_editorWindow_->bIsPlaying_)
		{
			// UPDATE
			// ------
			p_engine_->UpdateGame();
		}
		else if (p_editorWindow_->bIsReset)
		{
			// TODO : Add a reset of entities transform
			p_editorWindow_->bIsReset = false;
		}

		// TODO: Stop Using default Game Camera

		// RENDERING
		// ---------
		p_engine_->RunFrame();

		p_editorWindow_->DrawWindow(p_gameFBO->ID);
		p_engine_->UnBindFrame();
	}
}