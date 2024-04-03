#pragma once

class Window
{
public:
	Window();
	~Window();

	void* handle;
	void* GetWindowsHwnd();

	void Init(int _width, int _height, const char* _title);
	void StartFrame();
	void EndFrame();
	void CleanUp();

	int WindowShouldClose();
	void Close();
};