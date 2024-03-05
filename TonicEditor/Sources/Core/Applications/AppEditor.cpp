#include "Core/Applications/AppEditor.hpp"

#include <iostream>

#include"Core/EngineOverride/WindowOverride.hpp"
#include"Core/EngineOverride/GL_RHI.hpp"

using namespace Core::Applications;

AppEditor::AppEditor(const char* _appName, u32 _width, u32 _height)
{
	p_engine_ = new Engine();
	p_editorWindow_ = new EditorWindow();
	p_windowOW_ = new WindowOverride();
	p_GLRenderer_ = new GL_RHI();
	p_engine_->SetWindow(p_windowOW_);
	p_engine_->SetRenderer(p_GLRenderer_);

	p_engine_->Init(_appName, _width, _height);
};
AppEditor::~AppEditor()
{
	delete p_engine_;
	delete p_editorWindow_;
	delete p_windowOW_;
	delete p_GLRenderer_;
};
void AppEditor::Run()
{
	// ImGUI init
	p_editorWindow_->SetupImGui(p_engine_->GetWindow());

	// MainLoop
	while (!p_engine_->GetWindow()->IsClosing())
	{
		// START
		p_engine_->StartFrame();
		p_editorWindow_->StartFrame();
		p_editorWindow_->SetUpWindows();

		// ----------- OPENGL ON IMGUI WINDOW ------------------------------------------------
		p_editorWindow_->StartWindow();
		p_editorWindow_->EndWindow(p_engine_->GetTextureId());

		// Bind, Triangle, Unbind
		p_engine_->RunFrame();
		// ----------- END -------------------------------------------------------------------

		p_editorWindow_->EndFrame();
		p_engine_->EndFrame();
	}
	p_editorWindow_->CleanUp();
	p_engine_->CleanUp();
}

