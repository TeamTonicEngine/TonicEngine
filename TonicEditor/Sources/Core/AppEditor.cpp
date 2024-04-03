#include "Core/AppEditor.hpp"

#include <iostream>

Core::Applications::AppEditor::AppEditor(std::string _appName, u32 _width, u32 _height)
{
	p_engine_ = Engine::CreateEngine();
	if (p_engine_->Init(_appName.c_str(), _width, _height, true))
		DEBUG_SUCCESS("INITIALIZED ENGINE")
	else
		throw ("FAILED TO INITIALIZE ENGINE");

	p_editorWindow_ = new EditorWindow(GetGUI());
	if (p_editorWindow_)
		DEBUG_SUCCESS("INITIALIZED EDITOR WINDOW")
	else
		throw ("FAILED TO INITIALIZE EDITOR WINDOW");

	p_freeCamera_ = new LowRenderer::Cameras::FreeCamera(_width, _height);
	HEART::GetRenderer()->SetCamera(p_freeCamera_);
	p_freeCamera_->bUsed = true;
};

Core::Applications::AppEditor::~AppEditor()
{
	delete p_engine_;
	delete p_editorWindow_;
};

void Core::Applications::AppEditor::Run()
{
	// Main Loop
	while (!p_engine_->IsClosing())
	{
		// START
		p_engine_->StartFrame();
		p_editorWindow_->SetUpWindows();

		// RENDER
		p_editorWindow_->StartWindow();
		p_engine_->RunFrame();
		p_editorWindow_->DrawWindow(HEART::GetRenderer()->GetFrameBufferID());

		// END
		p_engine_->EndFrame();
	}

	// End of app
	p_engine_->Destroy();
}