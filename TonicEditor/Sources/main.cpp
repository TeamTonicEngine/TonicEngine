#include <iostream>

#include "Application.hpp"
using namespace Core::Application;

int main()
{
	Application* app = new Application("Tonic Editor");
	//Application* app = new Application("Tonic Editor", 1280, 720);

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