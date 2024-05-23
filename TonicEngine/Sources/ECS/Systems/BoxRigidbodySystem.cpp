#include "pch.hpp"

#include "ECS/Systems/BoxRigidbodySystem.hpp"

#include <ECS/Components/TransformComponent.hpp>
#include <ECS/Components/BoxRigidbodyComponent.hpp>

ECS::Systems::BoxRigidbodySystem::BoxRigidbodySystem()
{
	AddComponentSignature<Components::TransformComponent>();
	AddComponentSignature<Components::BoxRigidbodyComponent>();
}

void ECS::Systems::BoxRigidbodySystem::AddEntity(const EntityID _entity)
{
	entities_.insert(_entity);

	auto* p_em = ENGINE.ENT_MNGR;
	auto& transformComp = p_em->GetComponent<Components::TransformComponent>(_entity);
	auto& rigidbodyComp = p_em->GetComponent<Components::BoxRigidbodyComponent>(_entity);

	rigidbodyComp.Init(transformComp.scale, transformComp.position, transformComp.rotation);
}

void ECS::Systems::BoxRigidbodySystem::RemoveEntity(const EntityID _entity)
{
	entities_.erase(_entity);
	auto* p_em = ENGINE.ENT_MNGR;
	if (p_em->HasComponent<Components::BoxRigidbodyComponent>(_entity))
		p_em->GetComponent<Components::BoxRigidbodyComponent>(_entity).DestroyRigidbody();
}

void ECS::Systems::BoxRigidbodySystem::Update()
{
	if (ENGINE.SELF->bIsPlaying) return;

	auto* p_em = ENGINE.ENT_MNGR;
	for (auto& entity : entities_)
	{
		auto& transformComp = p_em->GetComponent<Components::TransformComponent>(entity);
		auto& rigidbodyComp = p_em->GetComponent<Components::BoxRigidbodyComponent>(entity);

		if (transformComp.HasChanged())
		{
			rigidbodyComp.center = transformComp.position;
			rigidbodyComp.rotationParent = transformComp.rotation;
			Maths::Vec3 extents = rigidbodyComp.GetExtents();
			rigidbodyComp.SetExtents({ extents.x * transformComp.scaleDiff_.x, extents.y * transformComp.scaleDiff_.y, extents.z * transformComp.scaleDiff_.z });
		}

		transformComp.position = rigidbodyComp.position - rigidbodyComp.centerOffset;
		transformComp.rotation = rigidbodyComp.rotationParent;
	}
}

void ECS::Systems::BoxRigidbodySystem::LateUpdate()
{
	if (!ENGINE.SELF->bIsPlaying) return;

	auto* p_em = ENGINE.ENT_MNGR;
	for (auto& entity : entities_)
	{
		auto& transformComp = p_em->GetComponent<Components::TransformComponent>(entity);
		auto& rigidbodyComp = p_em->GetComponent<Components::BoxRigidbodyComponent>(entity);

		if (transformComp.HasChanged())
		{
			rigidbodyComp.center = transformComp.position;
			rigidbodyComp.rotationParent = transformComp.rotation;
			Maths::Vec3 extents = rigidbodyComp.GetExtents();
			rigidbodyComp.SetExtents({ extents.x * transformComp.scaleDiff_.x, extents.y * transformComp.scaleDiff_.y, extents.z * transformComp.scaleDiff_.z });
		}

		// Its faster to set the values rather than check if the Rigidbody has changed i would argue
		transformComp.position = rigidbodyComp.position - rigidbodyComp.centerOffset;
		transformComp.rotation = rigidbodyComp.rotationParent;
		transformComp.scale = Maths::Vec3(rigidbodyComp.boxExtents_.x * transformComp.scale.x, rigidbodyComp.boxExtents_.y * transformComp.scale.y, rigidbodyComp.boxExtents_.z * transformComp.scale.z);
	}
}