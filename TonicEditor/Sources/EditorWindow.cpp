#include "EditorWindow.hpp"
#include "TonicEngine/Core/Window.hpp"

#include <stdio.h>

#include <iostream>

EditorWindow::EditorWindow()
{
    
}

EditorWindow::~EditorWindow()
{

}

void EditorWindow::SetupImGui(Core::Window* p_window)
{

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(p_window->GetHandle()), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init("#version 330");
}

void EditorWindow::SetUpWindows()
{
    InitUIWindow();
    
}

void EditorWindow::GetWindow()
{
}

bool EditorWindow::IsWindowClosed()
{
	if (isClose)
		return true;
	return false;
}

void EditorWindow::CloseWindow()
{
	isClose = true;
}

void EditorWindow::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void EditorWindow::InitUIWindow()
{
    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&window_class);

    // FLAGS
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiWindowFlags main_window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGuiWindowFlags window_flags =ImGuiWindowFlags_NoCollapse;
    static bool dockspaceOpen = true;

    viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.0f);


    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    if (ImGui::Begin("MainWindow", &dockspaceOpen, main_window_flags))
    {
        ImGui::PopStyleVar();

        // MENU BAR
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::MenuItem("Dummy");
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }


        // CREATE DOCK SPACE
        ImGuiID mainDockSpaceId = ImGui::GetID("MainDockSpace");
        const ImVec2 dockspace_size = ImGui::GetContentRegionAvail();
        ImGui::DockSpace(mainDockSpaceId, ImVec2(0.0f, 0.0f), dockspace_flags);

        // INITIAL WINDOWS
        

        ImGui::Begin("Game", &dockspaceOpen, window_flags);
        {

        }
        ImGui::End();

        ImGui::Begin("Property", &dockspaceOpen, window_flags);
        {

        }
        ImGui::End();

        ImGui::Begin("Scene Tree", &dockspaceOpen, window_flags);
        {

        }
        ImGui::End();

        ImGui::Begin("Project", &dockspaceOpen, window_flags);
        {

        }
        ImGui::End();

        ImGui::Begin("Debug", &dockspaceOpen, window_flags);
        {

        }
        ImGui::End();

        // Create Nodes
        static bool sFirstFrame = true;
        if (sFirstFrame)
        {
            sFirstFrame = false;

            ImGui::DockBuilderRemoveNode(mainDockSpaceId);
            ImGui::DockBuilderAddNode(mainDockSpaceId, ImGuiDockNodeFlags_DockSpace );
            ImGui::DockBuilderSetNodeSize(mainDockSpaceId, dockspace_size);

            ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(mainDockSpaceId, ImGuiDir_Down, 0.20f, NULL, &mainDockSpaceId);
            ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(mainDockSpaceId, ImGuiDir_Right, 0.15f, NULL, &mainDockSpaceId);
            ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(mainDockSpaceId, ImGuiDir_Left, 0.20f, NULL, &mainDockSpaceId);
            ImGuiID dock_id_top = ImGui::DockBuilderSplitNode(mainDockSpaceId, ImGuiDir_Up, 0.5f, NULL, &mainDockSpaceId);

            ImGuiID dock_id_bottom_right = ImGui::DockBuilderSplitNode(dock_id_bottom, ImGuiDir_Right, 0.2f, NULL, &dock_id_bottom);
            //ImGuiID dock_id_center = ImGui::DockBuilderSplitNode(mainDockSpaceId, ImGuiDir_COUNT, 1.f, NULL, &mainDockSpaceId);

            ImGui::DockBuilderDockWindow("Property", dock_id_right);
            ImGui::DockBuilderDockWindow("Scene Tree", dock_id_left);
            ImGui::DockBuilderDockWindow("Project", dock_id_bottom);
            ImGui::DockBuilderDockWindow("Debug", dock_id_bottom_right);
            ImGui::DockBuilderDockWindow("Scene", dock_id_top);
            ImGui::DockBuilderDockWindow("Game", dock_id_top);

            ImGui::DockBuilderFinish(mainDockSpaceId);
        }
    }

    ImGui::End();
}

void EditorWindow::Test()
{

}

void EditorWindow::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorWindow::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void EditorWindow::StartWindow()
{
    static bool dockspaceOpen = true;
    ImGui::Begin("Scene", &dockspaceOpen, ImGuiWindowFlags_NoCollapse);
    window_width = ImGui::GetContentRegionAvail().x;
    window_height = ImGui::GetContentRegionAvail().y;
    pos = ImGui::GetCursorScreenPos();
}

void EditorWindow::EndWindow(unsigned int texture_id)
{
    ImGui::GetWindowDrawList()->AddImage(
        (void*)texture_id,
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + window_width, pos.y + window_height),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImGui::End();
    ImGui::Render();
}