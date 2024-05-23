#pragma once

#include "types.hpp"
#include "EntityData.hpp"

namespace Core::Applications { class Engine; }

namespace ECS
{
	enum class TONIC_ENGINE_API ScriptType
	{
		Unset = 0u,
		PlayerController = 1u,
		LightController = 2u
		//Think about adding scripts here

	};

	class TONIC_ENGINE_API ScriptableEntity
	{
	public:
		virtual ScriptType GetScriptType() { return ScriptType::Unset; }

		std::string GetScriptTypeStr() const { return scriptTypeStr; }

		virtual ~ScriptableEntity() {}
		ScriptableEntity(EntityID _id);

		template<typename T>
		T& GetComponent();
		template<typename T>
		bool HasComponent();
	protected:
		std::string scriptTypeStr = "Unset";

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnStart() {}
		virtual void OnUpdate(float _deltaTime) {}
		virtual void OnFixedUpdate(float _fixedDeltaTime) {}

	private:
		std::shared_ptr<EntityData TONIC_ENGINE_API> p_entity_;

		//should be in a scene class
		friend class Core::Applications::Engine;
	};
}