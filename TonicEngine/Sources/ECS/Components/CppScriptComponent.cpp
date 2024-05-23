#include "pch.hpp"

#include "ECS/Components/CppScriptComponent.hpp"

void ECS::Components::CppScriptComponent::UnBind()
{
	scriptType = ScriptType::Unset;
	scriptTypeStr = "Unset";
	delete p_instance; p_instance = nullptr;
	InstantiateScript = nullptr;
	DestroyScript = nullptr;
}


// //WHY IT DOESN T WORK WHYYYYYYYYYYYYYYYYY
//template TONIC_ENGINE_API void ECS::Components::CppScriptComponent::Bind<PlayerController>();
//
//template TONIC_ENGINE_API void ECS::Components::CppScriptComponent::Bind<LightController>();



