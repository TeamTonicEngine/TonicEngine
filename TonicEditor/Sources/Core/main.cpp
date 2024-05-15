#include <iostream>

#include "Core/AppEditor.hpp"
using namespace Core::Applications;

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(93076); // Debugger should stop alloc nb

	std::system("chcp 65001");
	std::system("cls");

	std::cout << "argc = " << argc << ";\n";

	for (int i = 0; i < argc; i++)
	{
		std::cout << argv[i] << std::endl;
	}

	if (argc > 1)
	{
		std::string nPath = argv[1];
		nPath.erase(nPath.find_last_of('\\'));
		std::filesystem::current_path(nPath);
	}

	AppEditor app = AppEditor("Tonic Editor");

	try
	{
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}