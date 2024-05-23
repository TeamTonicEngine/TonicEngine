#pragma once
#include "ECS/Base/BaseSystem.hpp"

namespace ECS::Systems
{
	class TONIC_ENGINE_API SphereRigidbodySystem : public ECS::BaseSystem
	{
		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		SphereRigidbodySystem();
		~SphereRigidbodySystem() = default;

		virtual void AddEntity(const EntityID _entity) override;
		virtual void RemoveEntity(const EntityID _entity) override;

		void Update() override;
		void LateUpdate() override;
	};
}