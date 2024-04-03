#pragma once
#include "Window.hpp"
#include "ProjectHistory.hpp"
class App
{
private:
	Window window_;
	ProjectHistory projectHistory_;

	std::vector<FileData> showProject_;
	char searchText_[256] = "";
	char preSearchText_[256] = "";

	std::string OpenFile(const char* _filter);
	std::string SaveFile();

	void ReOrder();

public:
	App() {}
	~App() {}

	void Init();

	void Run();

	void CleanUp();

};