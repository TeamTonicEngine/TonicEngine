#include "App.hpp"
#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <glad/glad.hpp>


#include "Images.hpp"

std::string App::OpenFile(const char* _filter)
{
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };
    ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = static_cast<HWND>(window_.GetWindowsHwnd());
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = _filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if (GetOpenFileNameA(&ofn) == TRUE)
    {
        return ofn.lpstrFile;
    }
    return std::string();
}

std::string App::SaveFile()
{
    IFileDialog* pfd;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr)) {
        DWORD dwOptions;
        pfd->GetOptions(&dwOptions);
        pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);

        if (SUCCEEDED(pfd->Show(static_cast<HWND>(window_.GetWindowsHwnd())))) {
            IShellItem* psi;
            if (SUCCEEDED(pfd->GetResult(&psi))) {
                PWSTR pszFilePath;
                if (SUCCEEDED(psi->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
                    std::wstring wsFilePath(pszFilePath);
                    CoTaskMemFree(pszFilePath);
                    psi->Release();
                    pfd->Release();
                    return std::string(wsFilePath.begin(), wsFilePath.end());
                }
            }
        }
        pfd->Release();
    }
    return std::string();
}

void App::ReOrder()
{
    showProject_.clear();
    std::vector<FileData> t = projectHistory_.GetFiles(searchText_);
    showProject_ = t;
}    

void App::Init()
{
	std::cout << "Init Started\n";

    window_.Init(832, 580, "Tonic HUB");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // IF using Docking Branch
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window_.handle), true);  // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 330");

    gladLoadGL();

    InitStaticImages();

    projectHistory_.Open();

	std::cout << "Init Finished" << std::endl;

}

void App::Run()
{
    std::cout << "Run" << std::endl;

    showProject_ = projectHistory_.GetFiles();

    char newProjectName[256] = "";
    char newProjectPath[256]="";
    bool hasAPrev = false;
    char PrevPath[256] = "";
    RData nid = {1,nullptr};
    while(!window_.WindowShouldClose())
    {
        window_.StartFrame();


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
        ImGui::SetNextWindowClass(&window_class);

        // FLAGS
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_PassthruCentralNode;
        ImGuiWindowFlags main_window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
        static bool dockspaceOpen = true;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowBgAlpha(0.0f);


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        if (ImGui::Begin("MainWindow", &dockspaceOpen, main_window_flags))
        {
            float scroll = ImGui::GetScrollY();

            ImGui::PopStyleVar();

            // MENU BAR
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Import"))
                    {
                        std::string importProject;
                        importProject = OpenFile("Project File (*.tonic)\0*.TONIC\0");

                        if(importProject.size() > 0)
                            projectHistory_.ImportProject(importProject);
                        
                        ReOrder();
                    }
                    ImGui::Separator();
                    if (ImGui::MenuItem("Quit"))
                    {
                        window_.Close();
                    }
                    ImGui::EndMenu();
                }

                ImGui::PushItemWidth(200);
                ImGui::InputTextWithHint("##Search", "Search...", searchText_, IM_ARRAYSIZE(searchText_));
                ImGui::PopItemWidth();
                
                ImGui::SetCursorPos(ImVec2(740, scroll));

                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 18.000000);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.211764, 0.929411, 0.392156, 1.000000));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.000000, 0.000000, 0.000000, 1.000000));
                if(ImGui::Button("New Project"))
                    ImGui::OpenPopup("New Project");

                ImGui::PopStyleVar(1);
                ImGui::PopStyleColor(2);
                

                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                if (ImGui::BeginPopupModal("New Project", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    
                    nid = LoadResizeImage(PrevPath, 128, 128);

                    ImGui::Text("Name:");
                    ImGui::SameLine();

                    ImGui::InputTextWithHint("##projectName", "Name...", newProjectName, IM_ARRAYSIZE(newProjectName));
                    ImGui::Separator();


                    ImGui::Text("Location:");
                    ImGui::SameLine();

                    ImGui::InputTextWithHint("##projectPath", "Location...", newProjectPath, IM_ARRAYSIZE(newProjectPath));
                    ImGui::SameLine();
                    if (ImGui::Button("..."))
                    {
                        strcpy_s(newProjectPath, SaveFile().c_str());
                    }

                    ImGui::Separator();

                    ImGui::Text("PreviewImage:");
                    ImGui::SameLine();
                    ImGui::Checkbox("##R", &hasAPrev);
                    ImGui::SameLine();
                    ImGui::TextDisabled("(?)");
                    if (ImGui::BeginItemTooltip())
                    {
                        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                        ImGui::TextUnformatted("If not set will use the last image when editor is closed");
                        ImGui::PopTextWrapPos();
                        ImGui::EndTooltip();
                    }

                    ImGui::BeginDisabled(!hasAPrev);

                    ImGui::InputTextWithHint("##PrevPath", "Location...", PrevPath, IM_ARRAYSIZE(PrevPath));
                    ImGui::SameLine();
                    if (ImGui::Button("...##2"))
                    {
                        strcpy_s(PrevPath, OpenFile(nullptr).c_str());
                    }
                    ImGui::SameLine();
                    ImGui::Image(reinterpret_cast<ImTextureID>(nid.id), { 50, 50 });

                      
                    ImGui::EndDisabled();
                    ImGui::Separator();



                    ImGui::SetItemDefaultFocus();
                    if (ImGui::Button("Cancel", ImVec2(120, 0)))
                    {
                        ImGui::CloseCurrentPopup();
                        std::copy(std::begin(""), std::end(""), std::begin(newProjectName));
                        std::copy(std::begin(""), std::end(""), std::begin(newProjectPath));
                        std::copy(std::begin(""), std::end(""), std::begin(PrevPath));
                        hasAPrev = false;
                    }
                    ImGui::SameLine(0.0,100);
                    if (ImGui::Button("OK", ImVec2(120, 0)))
                    {
                        ImGui::CloseCurrentPopup();

                        if (nid.data && hasAPrev)
                            projectHistory_.CreateProject(newProjectPath, newProjectName, nid.width, nid.height,nid.data, nid.channels,nid.id);
                        else
                            projectHistory_.CreateProject(newProjectPath, newProjectName);

                        std::copy(std::begin(""), std::end(""), std::begin(newProjectName));
                        std::copy(std::begin(""), std::end(""), std::begin(newProjectPath));
                        std::copy(std::begin(""), std::end(""), std::begin(PrevPath));
                        hasAPrev = false;

                        ReOrder();
                    }

                    ImGui::EndPopup();
                }


                ImGui::EndMenuBar();
            }

            if (strcmp(searchText_, preSearchText_) != 0)
            {
                ReOrder();
                std::copy(std::begin(searchText_), std::end(searchText_), std::begin(preSearchText_));
            }

            int x = 0;
            int y = 0;
            for (int i = 0; i < showProject_.size(); [&]{
                i++;
                if(i%6 != 0)
                    x+= 136;
                else
                {
                    x=0;
                    y+=187;
                }
                }())
            {
            //Rect
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2( 16.000000 + x,
                 35.00000 + y - scroll),
                ImVec2( 136.000000 + x,
                     206.000000 + y - scroll),
                4294967295, 18.000000, 240);

            //ImageRect
            ImGui::GetWindowDrawList()->AddImageRounded(reinterpret_cast<ImTextureID>(showProject_[i].TextureId), ImVec2(25.000000 + x,
                43.000000 + y - scroll),
                ImVec2(126.000000 + x,
                    144.000000 + y - scroll),
                ImVec2(0, 0), ImVec2(1, 1), 4294967295, 12.000000, 240);

            //Project Name
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.000000, 0.000000, 0.000000, 1.000000));
            ImGui::SetCursorPos(ImVec2( 23.000000 + x
                ,  152.000000 + y));
            std::string projectName = showProject_[i].name;
            if (projectName.size() > 15)
            {
                projectName.erase(projectName.begin() + 12, projectName.end());
                projectName += "...";
            }
            ImGui::TextUnformatted(projectName.c_str());
            ImGui::PopStyleColor(1);
            
            //GoToLocation
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 18.000000);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.261438, 0.582853, 0.980392, 1.000000));
            ImGui::SetCursorPos(ImVec2(16.000000 + x
                , 176.000000 + y));
            std::string lbLabel = "Go To Location";
            lbLabel = lbLabel + "##" + showProject_[i].projectPath;
            if (ImGui::Button(lbLabel.c_str(), ImVec2(120.000000, 30.000000)))
            {
                std::string nPath = showProject_[i].projectPath;
                nPath.erase(nPath.find_last_of('\\')+1);

                ShellExecuteA(NULL, "open", nPath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
            }
            ImGui::PopStyleVar(1);
            ImGui::PopStyleColor(1);

            //Remove
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 18.000000);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0000, 0.4549, 0.4313, 1.0000));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8745, 0.4078, 0.3882, 1.0000));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0000, 0.5372, 0.5176, 1.0000));
            ImGui::SetCursorPos(ImVec2(118.0000 + x
                , 27.0000 + y));
            std::string rbLabel ="x##Remove" + showProject_[i].projectPath;//TODO Replace with an icon
            if (ImGui::Button(rbLabel.c_str(), ImVec2(26.00, 26.00)))
            {
                std::cout << "Project [" << showProject_[i].name << "] Removed.\n";
                projectHistory_.Remove(showProject_[i].projectPath);

                ReOrder();

                ImGui::PopStyleVar(1);
                ImGui::PopStyleColor(3);

                continue;
            }
            ImGui::PopStyleVar(1);
            ImGui::PopStyleColor(3);

            //Open
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0000, 0.0000, 0.0000, 0.0000));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0000, 0.0000, 0.0000, 0.0000));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0000, 0.0000, 0.0000, 0.0000));
            ImGui::SetCursorPos(ImVec2(16.000000 + x
                , 35.00000 + y));
            std::string gbLabel = "##Open" + showProject_[i].projectPath;
            if (ImGui::Button(gbLabel.c_str(), ImVec2(120.0000, 171.0000)))
            {
                std::cout << "Project [" << showProject_[i].name << "] Oppened.\n";

                std::string argPath = '\"' + showProject_[i].projectPath + '\"';

                ShellExecuteA(NULL, "open", "TonicEditor.exe", argPath.c_str(), NULL, SW_SHOWDEFAULT);
                projectHistory_.PlaceOnTop(showProject_[i].projectPath);
                ReOrder();
            }
            ImGui::PopStyleColor(3);

            //used to create an osset when scroll
            ImGui::SetCursorPos(ImVec2(0
                , 210.00000 + y));
            ImGui::TextUnformatted("");
            }

        }

        ImGui::End();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
        window_.EndFrame();
    }
    if (nid.data)
        delete[] nid.data;
}

void App::CleanUp()
{
	std::cout << "CleanUp Started\n";

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    projectHistory_.Save();
    window_.CleanUp();

	std::cout << "CleanUp Finished" << std::endl;
}