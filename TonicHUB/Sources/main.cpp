#include "App.hpp"

#ifndef NDEBUG
#include <crtdbg.h>
#endif

int main()
{
#ifndef NDEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(244);
#endif

	App app;

	app.Init();

	app.Run();

	app.CleanUp();

	return 0;
}