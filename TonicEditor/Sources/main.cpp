#include <iostream>

#include "Application.hpp"

int main()
{
	Core::Application* app = new Core::Application("Tonic Editor");
	//Core::Application* app = new Core::Application("Tonic Editor", 1280, 720);

	try
	{
		app->Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}