#pragma once

#include "TonicEngine/Core/Window.hpp"

class WindowOverride : public Core::Window
{
public:
	WindowOverride();

	void Init(const char* _name, u32 _width, u32 _height) override;

	s32_2 GetWindowSize() override;

	void Resized() override;

	bool IsClosing() override;

	void CleanUp() override;

	void StartFrame() override;

	void EndFrame() override;

	
	void ProcessInput() override;
private:

};