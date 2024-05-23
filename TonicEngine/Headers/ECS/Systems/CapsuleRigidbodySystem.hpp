#pragma once
#include "ECS/Base/BaseSystem.hpp"

namespace ECS::Systems
{
	class TONIC_ENGINE_API CapsuleRigidbodySystem : public ECS::BaseSystem
	{
		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		CapsuleRigidbodySystem();
		~CapsuleRigidbodySystem() = default;

		virtual void AddEntity(const EntityID _entity) override;
		virtual void RemoveEntity(const EntityID _entity) override;

		void Update() override;
		void LateUpdate() override;
	};
}