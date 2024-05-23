#pragma once

#include <functional>

#include "DLL_API.hpp"

#include "ECS/Base/ScriptableEntity.hpp"
#include "ECS/Base/ScriptFactory.hpp"



// Forward declaration
//namespace ECS { class ScriptableEntity; }

namespace Core::Applications { class Engine; }

namespace ECS::Components
{
	struct CppScriptComponent : public BaseComponent
	{
		ScriptableEntity* p_instance = nullptr;
		ScriptType scriptType = ScriptType::Unset;
		std::string scriptTypeStr = "Unset";


		//Signature				Func			 Parameter
		std::function<TONIC_ENGINE_API ScriptableEntity* ()> InstantiateScript;
		//ScriptableEntity* (* InstantiateScript)();
		void(*DestroyScript)	 (CppScriptComponent*) = nullptr;
		//Ex : void				  OnUpdate           (float _deltaTime)

		template<typename T>
		void Bind()
		{
			// Ensure T is derived from ScriptableEntity
			static_assert(std::is_base_of<ScriptableEntity, T>::value, "T must inherit from ScriptableEntity");

			// Create a temporary instance to call GetScriptType
			T tempInstance(entityID_);
			scriptType = tempInstance.GetScriptType();
			scriptTypeStr = tempInstance.GetScriptTypeStr();

			InstantiateScript = [this]() { return static_cast<ScriptableEntity*>(new T(entityID_)); };
			DestroyScript = [](CppScriptComponent* _p_csc) { delete _p_csc->p_instance; _p_csc->p_instance = nullptr; };
		}

		void TONIC_ENGINE_API UnBind();
		//Safety first ;)
		TONIC_ENGINE_API ~CppScriptComponent() { delete p_instance; }

		void TONIC_ENGINE_API Bind(ECS::ScriptType _scriptType)
		{
			UnBind();
			auto& factoryMap = ScriptFactories::GetScriptFactoryMap();
			auto it = factoryMap.find(_scriptType);
			if (it != factoryMap.end()) {
				scriptType = _scriptType;
				InstantiateScript = [this, it]() {
					p_instance = it->second(entityID_);
					return p_instance;
				};
				DestroyScript = [](CppScriptComponent* _p_csc) {
					delete _p_csc->p_instance;
					_p_csc->p_instance = nullptr;
				};
			}
		}

		//should be in a scene class
		friend class Core::Applications::Engine;		
	};
}