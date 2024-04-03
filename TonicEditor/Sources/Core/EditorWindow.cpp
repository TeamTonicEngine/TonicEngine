#include "Core/EditorWindow.hpp"

#include "Core/Engine.hpp"

#include <stdio.h>
#include <iostream>

#include <GLFW/glfw3.hpp>

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
	ImGuiWindowFlags mainWindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
	static bool dockspaceOpen = true;

	static bool gameWindow = true;
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

		// MENU BAR
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Dummy");
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
				//static float cColor[3];
				
				if (ImGui::ColorEdit3("Clear color", &cColor.x, ImGuiColorEditFlags_NoInputs))
				{
					Engine::GetRenderer()->ChangeClearColor(Color::CreateRGBAFloat(cColor.x, cColor.y, cColor.z, 1.f));
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Tools"))
			{
				ImGui::Checkbox("Game##opt", &gameWindow);
				ImGui::Checkbox("Property##opt", &propertyWindow);
				ImGui::Checkbox("Scene Tree##opt", &sceneTreeWindow);
				ImGui::Checkbox("Project##opt", &projectWindow);
				ImGui::Checkbox("Debug##opt", &debugWindow);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// CREATE DOCK SPACE
		ImGuiID mainDockSpaceId = ImGui::GetID("MainDockSpace");
		const ImVec2 dockspaceSize = ImGui::GetContentRegionAvail();
		ImGui::DockSpace(mainDockSpaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);

		// INITIAL WINDOWS

		if (gameWindow)
		{
			ImGui::Begin("Game", &gameWindow, windowFlags);
			{
			}
			ImGui::End();
		}

		if (propertyWindow)
		{
			ImGui::Begin("Property", &propertyWindow, windowFlags);
			{
			}
			ImGui::End();
		}

		if (sceneTreeWindow)
		{
			ImGui::Begin("Scene Tree", &sceneTreeWindow, windowFlags);
			{
			}
			ImGui::End();
		}

		if (projectWindow)
		{
			ImGui::Begin("Project", &projectWindow, windowFlags);
			{
				ImVec2 windowSize = ImGui::GetWindowSize();
				float scroll = ImGui::GetScrollY();

				int temp = std::floor(windowSize.x / 85);

				int x = 0;
				int y = 0;

				if (static_cast<Resources::Archi*>(p_projectArchi_)->parent)
				{
					if (ImGui::Button("..."))
						p_projectArchi_ = static_cast<Resources::Archi*>(p_projectArchi_)->parent;

					x = 34;
				}

				ImGui::SetCursorPos({ 8.f + x ,30.f });

				std::string t = "File : ";
				t += std::to_string(static_cast<Resources::Archi*>(p_projectArchi_)->file.size());
				ImGui::Text(t.c_str());

				x = 0;

				if (static_cast<Resources::Archi*>(p_projectArchi_)->subFolder.size() > 0)
				{
					for (int i = 0; i < static_cast<Resources::Archi*>(p_projectArchi_)->subFolder.size(); [&] {
						i++;
							if (i % temp != 0)
								x += 84;
							else
							{
								x = 0;
									y += 92;
							}
						}())
					{
						std::string dirName = "##" + static_cast<Resources::Archi*>(p_projectArchi_)->subFolder[i]->name;

						ImGui::SetCursorPos({ 8.f + x ,130.f + y });
						ImGui::Text(static_cast<Resources::Archi*>(p_projectArchi_)->subFolder[i]->name.c_str());

						ImGui::SetCursorPos({ 8.f + x ,52.f + y });
						if (ImGui::Button(dirName.c_str(), { 76,76 }))
						{
							p_projectArchi_ = static_cast<Resources::Archi*>(p_projectArchi_)->subFolder[i];
						}
						//ImGui::Image(0, { 76,76 });
					}
				}

				for (int i = 0; i < static_cast<Resources::Archi*>(p_projectArchi_)->file.size(); [&] {
					i++;
						if (i % temp != 0)
							x += 84;
						else
						{
							x = 0;
								y += 92;
						}
					}())
				{
					ImGui::SetCursorPos({ 8.f + x ,130.f + y });
					ImGui::Text(static_cast<Resources::Archi*>(p_projectArchi_)->file[i].c_str());

					ImGui::SetCursorPos({ 8.f + x ,52.f + y });
					ImGui::Image(0, { 76,76 });
				}
			}
			ImGui::End();
		}

		if (debugWindow)
		{
			ImGui::Begin("Debug", &debugWindow, windowFlags);
			{
				float fps = 1 / Engine::GetWindow()->GetDeltaTime();//We may want that in the Engine directly

				for (size_t i = 1; i <= 99; i++)
				{
					fpsHistory_[i - 1] = fpsHistory_[i];
				}
				fpsHistory_[99] = fps;

				std::string fpsText = "FPS: ";
				fpsText += std::to_string(fps);

				ImGui::Text(fpsText.c_str());
				ImGui::PlotHistogram("##Framerate", fpsHistory_, 99, 0, NULL, 0.f, 100.f, ImVec2(200, 100));
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

void Core::Applications::EditorWindow::StartWindow()
{
	static bool dockspaceOpen = true;
	ImGui::Begin("Scene", &dockspaceOpen, ImGuiWindowFlags_NoCollapse);
	width_ = ImGui::GetContentRegionAvail().x;
	height_ = ImGui::GetContentRegionAvail().y;
	pos_ = ImGui::GetCursorScreenPos();
}

void Core::Applications::EditorWindow::DrawWindow(unsigned texture_id)
{
	ImGui::Image(
		(void*)texture_id,
		ImGui::GetContentRegionAvail(),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	ImGui::End();
}