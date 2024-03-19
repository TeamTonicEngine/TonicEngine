#pragma once

#include "TonicEngine/Core/Window.hpp"
class GLFWwindow;

class WindowOverride : public Core::Window
{
public:
	WindowOverride();

	void Init(const char* _name, u32 _width, u32 _height) override;

	s32_2 GetWindowSize() override;

	FreeCamera* GetCamera() override;

	void Resized() override;

	bool IsClosing() override;

	void CleanUp() override;

	void StartFrame() override;

	void EndFrame() override;

	
	void ProcessInput() override;
private:
	void ProcessCameraInput();
	static void ScrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset);
	void ProcessMouseInput();

};