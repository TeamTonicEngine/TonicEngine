#include "pch.hpp"
#include "ECS/Base/ScriptFactory.hpp"

ECS::ScriptFactoryMap& ECS::ScriptFactories::GetScriptFactoryMap()
{
	static ScriptFactoryMap factoryMap;
	return factoryMap;
}
