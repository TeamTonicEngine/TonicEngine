#pragma once
#include "ECS/Base/ScriptableEntity.hpp"

#include <functional>
#include <map>
#include <memory>

namespace ECS
{
	enum class ScriptType;

	using ScriptFactoryFunction = std::function<ECS::ScriptableEntity* (ECS::EntityID)>;
	using ScriptFactoryMap = std::map<ECS::ScriptType, ScriptFactoryFunction>;

	namespace ScriptFactories
	{
		TONIC_ENGINE_API ScriptFactoryMap& GetScriptFactoryMap();

		template<typename T>
		ECS::ScriptableEntity* CreateScriptInstance(ECS::EntityID id) {
			return new T(id);
		}

		template<typename T>
		void RegisterScript(ECS::ScriptType scriptType) {
			GetScriptFactoryMap()[scriptType] = &CreateScriptInstance<T>;
		}
	}
}