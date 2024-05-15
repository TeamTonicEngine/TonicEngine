#pragma once

#include "ECS/Base/BaseSystem.hpp"

namespace ECS::Components
{
	struct TransformComponent;
}
namespace ECS::Systems
{
	class TransformSystem : public ECS::BaseSystem
	{
	public:
		TONIC_ENGINE_API TransformSystem();
		TONIC_ENGINE_API ~TransformSystem() {}

		const bool Init() override;
		void TONIC_ENGINE_API Update() override;
		void TONIC_ENGINE_API LateUpdate() override;

	protected:
		std::set<EntityID> changedEntities_;
		void RecursiveUpdate(EntityID _entity, Components::TransformComponent* _p_transform);
	};
}