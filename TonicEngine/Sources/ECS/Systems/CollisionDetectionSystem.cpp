#include "pch.hpp"
#include "ECS/Systems/CollisionDetectionSystem.hpp"

#include "ECS/Components.hpp"

#include "Maths/Maths.hpp"

ECS::Systems::CollisionDetectionSystem::CollisionDetectionSystem()
{
	AddComponentSignature<Components::TransformComponent>();
	AddComponentSignature<Components::IColliderComponent>();
}


void ECS::Systems::CollisionDetectionSystem::Update()
{
	ECS::EntityManager* p_em = ENGINE.ENT_MNGR;

	for (auto& entity : entities_)
	{
		Components::TransformComponent& transform = p_em->GetComponent<Components::TransformComponent>(entity);
		Components::IColliderComponent& collider = p_em->GetComponent<Components::IColliderComponent>(entity);

		if (transform.HasChanged())
		{
			//TODO: Test here if the transform changed, or Event
			collider.bounds.center += transform.position;
			//Here for collisionData
		}
	}
}