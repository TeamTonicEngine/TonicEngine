#include "pch.hpp"

bool APIENTRY DllMain(HMODULE _hModule, DWORD _ul_reason_for_call, LPVOID _lpReserved)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(20290); // Debugger should stop alloc nb

	return true;
}