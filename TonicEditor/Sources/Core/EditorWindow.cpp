#pragma once
#include "Core/EditorWindow.hpp"
#include "Core/Utils.hpp"

#include <stdio.h>
#include <iostream>

#include <GLFW/glfw3.hpp>

//Open the windows file saver --------------------------------------------------------------------------------------------------
#include <windows.h>
#include <commdlg.h>
std::string SaveFile(const char* _filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = static_cast<HWND>(ENGINE.WDW->GetWindowsHwnd());
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetSaveFileNameA(&ofn) == TRUE)
	{
		return ofn.lpstrFile;
	}
	return std::string();
}
//------------------------------------------------------------------------------------------------------------------------------

bool EndsWithIc3(const std::string& str) {
	const std::string extension = ".ic3";
	if (str.length() >= extension.length()) {
		return (0 == str.compare(str.length() - extension.length(), extension.length(), extension));
	}
	else {
		return false;
	}
}

Core::Applications::EditorWindow::EditorWindow(ImGuiEngine* _p_imguiEngine) { p_imguiEngine_ = _p_imguiEngine; p_projectArchi_ = Engine::GetResourceManager()->p_files; }

Core::Applications::EditorWindow::~EditorWindow() {}

void Core::Applications::EditorWindow::SetUpWindows()
{
	ImGui::SetCurrentContext((ImGuiContext*)p_imguiEngine_->GetCurrentContext());
	InitUIWindow();
}

void Core::Applications::EditorWindow::InitUIWindow()
{
	ImGuiWindowClass windowClass;
	windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&windowClass);

	// FLAGS
	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags mainWindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus;
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
	static bool dockspaceOpen = true;

	static bool propertyWindow = true;
	static bool sceneTreeWindow = true;
	static bool projectWindow = true;
	static bool debugWindow = true;

	p_viewport_ = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(p_viewport_->Pos);
	ImGui::SetNextWindowSize(p_viewport_->Size);
	ImGui::SetNextWindowViewport(p_viewport_->ID);
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	if (ImGui::Begin("MainWindow", &dockspaceOpen, mainWindowFlags))
	{
		ImGui::PopStyleVar();

		//Menu Bar ---------------------------------------------------------------------------------------------------------------------
		if (ImGui::BeginMenuBar())
		{
			float barWidth = ImGui::GetCurrentWindow()->MenuBarRect().GetWidth();

			if (ImGui::BeginMenu("File"))
			{
				if (curentScenePath.size() > 0)
				{
					if (ImGui::MenuItem("Save Scene"))
						Resources::Scene::SaveFile(curentScenePath.c_str());
				}
				else
					ImGui::TextDisabled("Save Scene");

				if (ImGui::MenuItem("Save Scene As..."))
				{
					std::string newPath;
					newPath = SaveFile("Project File (*.ic3)\0*.IC3\0");

					if (newPath.size() > 0)
					{
						if (!EndsWithIc3(newPath))
						{
							newPath += ".ic3";
						}

						curentScenePath = newPath;
						Resources::Scene::SaveFile(curentScenePath.c_str());
					}
				}

				ImGui::Separator();
				if (ImGui::MenuItem("Exit"))
					Engine::GetWindow()->IsClosing(true);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::MenuItem("Dummy");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::Text("Settings:");

				f32_4 cColor = Engine::GetRenderer()->GetClearColor().ToFloat();

				if (ImGui::ColorEdit3("Clear color", &cColor.x, ImGuiColorEditFlags_NoInputs))
				{
					Engine::GetRenderer()->ChangeClearColor(TNCColor::CreateRGBAFloat(cColor.x, cColor.y, cColor.z, 1.f));
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				ImGui::Checkbox("Game##opt", &gameWindow);
				ImGui::Checkbox("Property##opt", &propertyWindow);
				ImGui::Checkbox("Scene Tree##opt", &sceneTreeWindow);
				ImGui::Checkbox("Project##opt", &projectWindow);
				ImGui::Checkbox("Debug##opt", &debugWindow);
				ImGui::EndMenu();
			}


			//Game state button ------------------------------------------------------------------------------------------------------------
			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.5, 0.5, 0.5, 0.5 });

			ImGui::SetCursorPosX((barWidth * 0.5f) - 45.f);
			if (ImGui::ImageButton(
				(ImTextureID)(u64)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\play.png")->ID,
				{ 19,19 }, { 0,1 }, { 1,0 }, 0)
				)
			{
				gameWindow = true;
				ENGINE.SELF->bIsPlaying = true;
				ENGINE.SELF->bIsStarting = true;
			}

			ImGui::SetCursorPosX(barWidth * 0.5f);
			if (ImGui::ImageButton(
				(ImTextureID)(u64)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\pause.png")->ID,
				{ 19,19 }, { 0,1 }, { 1,0 }, 0)
				)
			{
				// PAUSE
				ENGINE.SELF->bIsPlaying = false;
			}

			ImGui::SetCursorPosX((barWidth * 0.5f) + 45.f);
			if (ImGui::ImageButton(
				(ImTextureID)(u64)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\stop.png")->ID,
				{ 19,19 }, { 0,1 }, { 1,0 }, 0)
				)
			{
				//STOP
				ENGINE.SELF->bIsReset = true;
				ENGINE.SELF->bIsPlaying = false;
			}

			ImGui::PopStyleColor(2);
			//------------------------------------------------------------------------------------------------------------------------------


			ImGui::EndMenuBar();
		}
		//------------------------------------------------------------------------------------------------------------------------------

		// CREATE DOCK SPACE
		ImGuiID mainDockSpaceId = ImGui::GetID("MainDockSpace");
		const ImVec2 dockspaceSize = ImGui::GetContentRegionAvail();
		ImGui::DockSpace(mainDockSpaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);

		// INITIAL WINDOWS

		if (propertyWindow)
		{
			ImGui::Begin("Property", &propertyWindow, windowFlags);
			{
				bPropertyHovered = ImGui::IsWindowHovered();
				DrawProperty(selectedId);
			}
			ImGui::End();
		}

		if (sceneTreeWindow)
		{
			ImGui::Begin("Scene Tree", &sceneTreeWindow, windowFlags);
			{
				bSceneTreeHovered = ImGui::IsWindowHovered();

				DrawTree(ECS::ROOT_ENTITY_ID);

				ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.5, 0.5, 0.5, 0.5 });

				if (ImGui::ImageButton((ImTextureID)(u64)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\plus.png")->ID,
					{ 19,19 }, { 0,1 }, { 1,0 }, 0))
				{
					selectedId = ENGINE.ENT_MNGR->AddNewEntity();
				}

				ImGui::PopStyleColor(2);
			}
			ImGui::End();
		}

		if (projectWindow)
		{
			ImGui::Begin("Project", &projectWindow, windowFlags);
			{
				bProjectHovered = ImGui::IsWindowHovered();

				DrawContentBrowser();
			}
			ImGui::End();
		}

		if (debugWindow)
		{
			ImGui::Begin("Debug", &debugWindow, windowFlags);
			{
				bDebugHovered = ImGui::IsWindowHovered();

				float fps = 1 / Engine::GetWindow()->GetDeltaTime();//We may want that in the Engine directly
				static float moySta = 0;
				float moy = 0;

				for (size_t i = 1; i <= 99; i++)
				{
					fpsHistory_[i - 1] = fpsHistory_[i];
				}
				fpsHistory_[99] = fps;

				//TODO We want this to run for like the 1000 first frame or so.
				for (const float& value : fpsHistory_)
				{
					moy += value;
				}
				moy /= 100;
				if (moy + 20 < moySta || moy > moySta + 20)
					moySta = moy;

				std::string fpsText = "FPS: ";
				fpsText += std::to_string(fps);

				ImGui::Text(fpsText.c_str());
				ImGui::PlotHistogram("##Framerate", fpsHistory_, 99, 0, NULL, 0.f, moySta * 2, ImVec2(200, 100));
			}
			ImGui::End();
		}

		// Create Nodes
		static bool s_firstFrame = true;
		if (s_firstFrame)
		{
			s_firstFrame = false;
			FirstFrame(mainDockSpaceId, dockspaceSize);
		}
	}

	ImGui::End();
}

void Core::Applications::EditorWindow::FirstFrame(ImGuiID& _mainDockSpaceId, const ImVec2& _dockspaceSize)
{
	ImGui::DockBuilderRemoveNode(_mainDockSpaceId);
	ImGui::DockBuilderAddNode(_mainDockSpaceId, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(_mainDockSpaceId, _dockspaceSize);

	ImGuiID dockIdBottom = ImGui::DockBuilderSplitNode(_mainDockSpaceId, ImGuiDir_Down, 0.20f, NULL, &_mainDockSpaceId);
	ImGuiID dockIdRight = ImGui::DockBuilderSplitNode(_mainDockSpaceId, ImGuiDir_Right, 0.15f, NULL, &_mainDockSpaceId);
	ImGuiID dockIdLeft = ImGui::DockBuilderSplitNode(_mainDockSpaceId, ImGuiDir_Left, 0.20f, NULL, &_mainDockSpaceId);
	ImGuiID dockIdTop = ImGui::DockBuilderSplitNode(_mainDockSpaceId, ImGuiDir_Up, 0.5f, NULL, &_mainDockSpaceId);

	ImGuiID dockIdBottomRight = ImGui::DockBuilderSplitNode(dockIdBottom, ImGuiDir_Right, 0.2f, NULL, &dockIdBottom);

	ImGui::DockBuilderDockWindow("Property", dockIdRight);
	ImGui::DockBuilderDockWindow("Scene Tree", dockIdLeft);
	ImGui::DockBuilderDockWindow("Project", dockIdBottom);
	ImGui::DockBuilderDockWindow("Debug", dockIdBottomRight);

	ImGui::DockBuilderDockWindow("Scene", _mainDockSpaceId);
	ImGui::DockBuilderDockWindow("Game", _mainDockSpaceId);

	ImGui::DockBuilderFinish(_mainDockSpaceId);
}

void Core::Applications::EditorWindow::StartWindow(std::string _name, bool* _bOpen)
{
	ImGui::Begin(_name.c_str(), _bOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing);
	width_ = ImGui::GetContentRegionAvail().x;
	height_ = ImGui::GetContentRegionAvail().y;
	pos_ = ImGui::GetCursorScreenPos();
	ImGui::GetForegroundDrawList();

	if (_name == "Scene")
	{
		bIsSceneFocused = ImGui::IsWindowFocused();
		bSceneHovered = ImGui::IsWindowHovered();
	}
	else if (_name == "Game")
	{
		bIsGameFocused = ImGui::IsWindowFocused();
		bGameHovered = ImGui::IsWindowHovered();
	}
}

void Core::Applications::EditorWindow::FocusHoveredWindow()
{
	bIsSceneFocused = bIsGameFocused = false;

	if (bPropertyHovered)
		ImGui::FocusWindow(ImGui::FindWindowByID(ImHashStr("Property")));
	else if (bSceneTreeHovered)
		ImGui::FocusWindow(ImGui::FindWindowByID(ImHashStr("Scene Tree")));
	else if (bProjectHovered)
		ImGui::FocusWindow(ImGui::FindWindowByID(ImHashStr("Project")));
	else if (bDebugHovered)
		ImGui::FocusWindow(ImGui::FindWindowByID(ImHashStr("Debug")));
	else if (bSceneHovered)
	{
		ImGui::FocusWindow(ImGui::FindWindowByID(ImHashStr("Scene")));
		bIsSceneFocused = true;
	}
	else if (bGameHovered)
	{
		ImGui::FocusWindow(ImGui::FindWindowByID(ImHashStr("Game")));
		bIsGameFocused = true;
	}
}

void Core::Applications::EditorWindow::DrawWindow(u64 texture_id)
{
	ImGui::Image(
		reinterpret_cast<ImTextureID>(texture_id),
		ImGui::GetContentRegionAvail(),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);
}