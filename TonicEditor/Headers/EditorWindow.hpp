#pragma once

class EditorWindow
{
private:

	bool isClose = true;

public:
	EditorWindow();
	~EditorWindow();

	void Init(void* p_window);
	void CreateWindow();
	void GetWindow();
	bool IsWindowClosed();
	void CloseWindow();

	void Frame();



};


