#pragma once
#include "Core/AppEditor.hpp"

#include <iostream>

#include <LowRenderer/Cameras/FreeCamera.hpp>

#include "../Assets/Script/PlayerController.hpp"
#include "../Assets/Script/LightController.hpp"


#include "ECS/Components.hpp"

#include <ImGuizmo/ImGuizmo.h>

Core::Applications::AppEditor::AppEditor(std::string _appName, u32 _width, u32 _height)
{
	p_engine_ = Engine::CreateEngine();

	if (p_engine_->Init(_appName.c_str(), _width, _height, true))
		DEBUG_SUCCESS("INITIALIZED ENGINE")
	else
		throw ("FAILED TO INITIALIZE ENGINE");

	p_sceneFBO_ = p_engine_->InitFBO();
	p_gameFBO_ = p_engine_->InitFBO();

	Init();

	p_editorWindow_ = new EditorWindow(ENGINE.GUI);
	if (p_editorWindow_)
		DEBUG_SUCCESS("INITIALIZED EDITOR WINDOW")
	else
		throw ("FAILED TO INITIALIZE EDITOR WINDOW");
}

Core::Applications::AppEditor::~AppEditor()
{
	delete p_sceneFBO_;
	delete p_gameFBO_;
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

	p_RM->Create<Resources::Font>("StaticAssets\\Fonts\\arial.ttf", false, false);
	p_RM->Create<Resources::Font>("StaticAssets\\Fonts\\LITTLE NIGHTMARE.ttf");
	p_RM->Create<Resources::Font>("StaticAssets\\Fonts\\ghoulishintent.ttf");
	p_RM->Create<Resources::Font>("StaticAssets\\Fonts\\Jesus Franco.ttf");

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

	RegisterScripts();

	HandleEvent();
}
void Core::Applications::AppEditor::RegisterScripts() 
{
	ECS::ScriptFactories::RegisterScript<PlayerController>(ECS::ScriptType::PlayerController);
	ECS::ScriptFactories::RegisterScript<LightController>(ECS::ScriptType::LightController);
}

void Core::Applications::AppEditor::HandleEvent()
{
	//Focus an imgui window when right click
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Mouse::ButtonRight, [&]()
		{
			p_editorWindow_->FocusHoveredWindow();
		});

	//Save the cursor position when clicked on the scene
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Mouse::ButtonRight, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused)
			{
				input_.mouse.pos = ENGINE.INP_MNGR->mousePosition;
			}
		});

	//Hide the cursor when right click is hold on the scene
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Hold, Core::Applications::Mouse::ButtonRight, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused)
			{
				ENGINE.INP_MNGR->SetCursorVisibility(false);
			}
		});

	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Released, Core::Applications::Mouse::ButtonRight, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused)
			{
				ENGINE.INP_MNGR->SetCursorVisibility(true);
			}
		});

	//move the editor cam -----------------------------------------------------------------------------------------------------------------------------------------
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Hold, Core::Applications::Keyboard::KeyW, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused && ENGINE.INP_MNGR->keys[Core::Applications::Mouse::ButtonRight] == Core::Applications::EventType::Hold)
			{
				input_.cameraInput.bMoveForward = true;
			}
		});

	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Hold, Core::Applications::Keyboard::KeyA, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused && ENGINE.INP_MNGR->keys[Core::Applications::Mouse::ButtonRight] == Core::Applications::EventType::Hold)
			{
				input_.cameraInput.bMoveLeft = true;
			}
		});

	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Hold, Core::Applications::Keyboard::KeyD, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused && ENGINE.INP_MNGR->keys[Core::Applications::Mouse::ButtonRight] == Core::Applications::EventType::Hold)
			{
				input_.cameraInput.bMoveRight = true;
			}
		});

	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Hold, Core::Applications::Keyboard::KeyS, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused && ENGINE.INP_MNGR->keys[Core::Applications::Mouse::ButtonRight] == Core::Applications::EventType::Hold)
			{
				input_.cameraInput.bMoveBackward = true;
			}
		});

	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Hold, Core::Applications::Keyboard::KeyE, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused && ENGINE.INP_MNGR->keys[Core::Applications::Mouse::ButtonRight] == Core::Applications::EventType::Hold)
			{
				input_.cameraInput.bMoveUp = true;
			}
		});

	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Hold, Core::Applications::Keyboard::KeyQ, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused && ENGINE.INP_MNGR->keys[Core::Applications::Mouse::ButtonRight] == Core::Applications::EventType::Hold)
			{
				input_.cameraInput.bMoveDown = true;
			}
		});
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------

	//rotate editor cam
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::MoveMouse, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused && ENGINE.INP_MNGR->keys[Core::Applications::Mouse::ButtonRight] == Core::Applications::EventType::Hold)
			{
				f32_2 newPos = ENGINE.INP_MNGR->mousePosition;

				input_.cameraInput.deltaX = -(newPos.x - input_.mouse.pos.x);
				input_.cameraInput.deltaY = (newPos.y - input_.mouse.pos.y);

				ENGINE.INP_MNGR->SetCursorPos(input_.mouse.pos);
			}
		});

	//The Zoom of the editor cam
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::ScrollChange, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused && ENGINE.INP_MNGR->keys[Core::Applications::Mouse::ButtonRight] == Core::Applications::EventType::Hold)
			{
				input_.cameraInput.zoom = ENGINE.INP_MNGR->scrollOffset;
			}
		});

	//Set the gizmo to rotation
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Keyboard::KeyR, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused)
			{
				p_editorWindow_->gizmod = GizmoMod::ROTATE;
			}
		});

	//Set the gizmo to translate
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Keyboard::KeyT, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused)
			{
				p_editorWindow_->gizmod = GizmoMod::TRANSLATE;
			}
		});

	//Set the gizmo to scale
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Keyboard::KeyY, [&]()
		{
			if (p_editorWindow_->bIsSceneFocused)
			{
				p_editorWindow_->gizmod = GizmoMod::SCALE;
			}
		});

	//Open the popup for the the content browser
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Mouse::ButtonRight, [&]()
		{
			if (p_editorWindow_->bProjectHovered)
			{
				p_editorWindow_->bOpenProjectPopup = true;
			}
		});

	//Save the Scene on ctrl + S
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Hold, Core::Applications::Keyboard::KeyLeftControl, [&]()
		{
			if (ENGINE.INP_MNGR->keys[Core::Applications::Keyboard::KeyS] == Core::Applications::EventType::Pressed && p_editorWindow_->curentScenePath.size() > 0)
			{
				Resources::Scene::SaveFile(p_editorWindow_->curentScenePath.c_str());
			}
		});

	//"Launch" the game when F10
	ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Released, Core::Applications::Keyboard::KeyF10, [&]()
		{
			bGame = !bGame;
			ENGINE.WDW->SetFullScreen(bGame);
			if (bGame)
			{
				if (ENGINE.SELF->bIsPlaying)
					return;
				ENGINE.SELF->bIsStarting = true;
				ENGINE.SELF->bIsPlaying = true;
			}
			else
			{
				ENGINE.SELF->bIsPlaying = false;
			}
		});
}

#include <ECS/Components.hpp> //TODO Test ECS TODELETE

void Core::Applications::AppEditor::Run()
{
	auto bakupakumeshu = ENGINE.RES_MNGR->Create<Resources::Mesh>("Assets\\Meshes\\backpack.obj");
	auto diffuse = ENGINE.RES_MNGR->Create<Resources::Texture>("Assets\\Textures\\Back\\backpack_diffuse.jpg");

	auto specular = ENGINE.RES_MNGR->Create<Resources::Texture>("Assets\\Textures\\Back\\backpack_specular.jpg");

	auto normal = ENGINE.RES_MNGR->Create<Resources::Texture>("Assets\\Textures\\Back\\backpack_normal.png");
	auto roughness = ENGINE.RES_MNGR->Create<Resources::Texture>("Assets\\Textures\\Back\\backpack_roughness.jpg");
	auto ao = ENGINE.RES_MNGR->Create<Resources::Texture>("Assets\\Textures\\Back\\backpack_ao.jpg");

	//Script test
	auto p_em = ENGINE.ENT_MNGR;
	ECS::EntityID e0 = p_em->AddNewEntity();
	p_em->AddComponent<ECS::Components::TransformComponent>(e0);
	LowRenderer::Cameras::Camera cam = LowRenderer::Cameras::Camera(ENGINE.WDW->GetWindowSize().x, ENGINE.WDW->GetWindowSize().y);
	p_em->AddComponent<ECS::Components::CameraComponent>(e0, cam);
	p_em->AddComponent<ECS::Components::CppScriptComponent>(e0).Bind<PlayerController>();

	////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Load the game scene to add scripts

	ENGINE.RES_MNGR->Create<Resources::Scene>("Assets\\Scenes\\scene.ic3", false, false)->Use();
	p_editorWindow_->curentScenePath = "Assets\\Scenes\\scene.ic3";
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (!p_engine_->IsClosing())
	{
		///////////////////////////////////////////////////////////////////////////////
		//UPDATE

		// START

		if (bGame)
		{
			p_engine_->StartFrame();

			//-- GAME -------------------------------------------------------------------------------
			if (ENGINE.SELF->bIsStarting)
			{
				// UPDATE
				// ------
				p_engine_->StartGame();
				ENGINE.SELF->bIsStarting = false;
			}

			p_engine_->UpdateGame();

			p_engine_->RunFrame();

			//---------------------------------------------------------------------------------------
		}
		else
		{
			UpdateCurrentCamera();
			p_engine_->UpdateScene();

			p_engine_->StartFrame();
			p_editorWindow_->SetUpWindows();

			PlayScene();
			PlayGame();
		}

		// END
		p_engine_->EndFrame();
	}
}

void Core::Applications::AppEditor::PlayScene()
{
	ImGuizmo::BeginFrame();
	p_engine_->BindFrame(p_sceneFBO_);
	{
		// CREATE THE SCENE WINDOW
		// -----------------------
		p_editorWindow_->StartWindow("Scene");
		ImVec2 size = ImGui::GetWindowSize();
		ImVec2 pos = ImGui::GetWindowPos();
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

		//ENGINE.RDR

		ENGINE.SELF->p_freeCamera->aspect = static_cast<float>(size.x) / size.y;

		ENGINE.SELF->p_freeCamera->SetProjection();

		ENGINE.SELF->p_freeCamera->bProjChanged = true;

		// RENDERING
		// ---------
		p_engine_->RunFrame(true);
		p_editorWindow_->DrawWindow(p_sceneFBO_->ID);

		if (p_editorWindow_->selectedHasTransform)
		{
			ImGuizmo::Manipulate(ENGINE.SELF->p_freeCamera->view.data, ENGINE.SELF->p_freeCamera->projection.data, (ImGuizmo::OPERATION)p_editorWindow_->gizmod, ImGuizmo::WORLD, p_editorWindow_->selectedTransform.data);

			p_editorWindow_->gizmoUsed = ImGuizmo::IsUsing();
		}

		ImGui::End();
	}
	p_engine_->UnBindFrame();
}

void Core::Applications::AppEditor::PlayGame()
{
	if (p_editorWindow_->gameWindow)
	{
		p_engine_->BindFrame(p_gameFBO_);
		// CREATE THE GAME WINDOW
		// ----------------------
		p_editorWindow_->StartWindow("Game", &p_editorWindow_->gameWindow);

		ImVec2 size = ImGui::GetWindowSize();

		ENGINE.SELF->p_freeCamera->aspect = static_cast<float>(size.x) / size.y;

		ENGINE.SELF->p_freeCamera->SetProjection();

		ENGINE.SELF->p_freeCamera->bProjChanged = true;

		// PLAY
		// ----
		if (ENGINE.SELF->bIsStarting)
		{
			// UPDATE
			// ------
			p_engine_->StartGame();
			ENGINE.SELF->bIsStarting = false;
		}
		if (ENGINE.SELF->bIsPlaying)
		{
			// UPDATE
			// ------
			p_engine_->UpdateGame();
		}
		else if (ENGINE.SELF->bIsReset)
		{
			// TODO : Add a reset of entities transform
			ENGINE.SELF->bIsReset = false;
		}
		// TODO: Stop Using default Game Camera

		// RENDERING
		// ---------
		p_engine_->RunFrame();

		p_editorWindow_->DrawWindow(p_gameFBO_->ID);
		ImGui::End();

		p_engine_->UnBindFrame();
	}
}

void Core::Applications::AppEditor::UpdateCurrentCamera()
{
	if (!ENGINE.RDR->GetCamera())
	{
		DEBUG_ERROR("Core::Renderer::OpenGLWrapper::UpdateCurrentCamera called but no current camera is set");
		return;
	}

	ENGINE.SELF->p_freeCamera->ProcessInput(ENGINE.SELF->GetDeltaTime(), input_.cameraInput);
	input_.cameraInput.bMoveForward = false;
	input_.cameraInput.bMoveBackward = false;
	input_.cameraInput.bMoveRight = false;
	input_.cameraInput.bMoveLeft = false;
	input_.cameraInput.bMoveUp = false;
	input_.cameraInput.bMoveDown = false;
	input_.cameraInput.deltaX = 0.f;
	input_.cameraInput.deltaY = 0.f;
	ENGINE.SELF->p_freeCamera->Update();
}