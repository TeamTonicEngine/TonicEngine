#include "pch.hpp"

#include "ECS/Systems/CapsuleRigidbodySystem.hpp"

#include <ECS/Components/TransformComponent.hpp>
#include <ECS/Components/CapsuleRigidbodyComponent.hpp>

ECS::Systems::CapsuleRigidbodySystem::CapsuleRigidbodySystem()
{
	AddComponentSignature<Components::TransformComponent>();
	AddComponentSignature<Components::CapsuleRigidbodyComponent>();
}

void ECS::Systems::CapsuleRigidbodySystem::AddEntity(const EntityID _entity)
{
	entities_.insert(_entity);

	auto* p_em = ENGINE.ENT_MNGR;
	auto& transformComp = p_em->GetComponent<Components::TransformComponent>(_entity);
	auto& rigidbodyComp = p_em->GetComponent<Components::CapsuleRigidbodyComponent>(_entity);

	rigidbodyComp.Init(transformComp.scale, transformComp.position, transformComp.rotation);
}

void ECS::Systems::CapsuleRigidbodySystem::RemoveEntity(const EntityID _entity)
{
	entities_.erase(_entity);
	auto* p_em = ENGINE.ENT_MNGR;
	if (p_em->HasComponent<Components::CapsuleRigidbodyComponent>(_entity))
		p_em->GetComponent<Components::CapsuleRigidbodyComponent>(_entity).DestroyRigidbody();
}

void ECS::Systems::CapsuleRigidbodySystem::Update()
{
	if (ENGINE.SELF->bIsPlaying) return;

	auto* p_em = ENGINE.ENT_MNGR;
	for (auto& entity : entities_)
	{
		auto& transformComp = p_em->GetComponent<Components::TransformComponent>(entity);
		auto& rigidbodyComp = p_em->GetComponent<Components::CapsuleRigidbodyComponent>(entity);

		if (transformComp.HasChanged())
		{
			rigidbodyComp.center = transformComp.position;
			rigidbodyComp.rotationParent = transformComp.rotation;
		}

		// Its faster to set the values this than check if the Rigidbody has change i would argue
		transformComp.position = rigidbodyComp.position - rigidbodyComp.centerOffset;
		transformComp.rotation = rigidbodyComp.rotationParent;
	}
}

void ECS::Systems::CapsuleRigidbodySystem::LateUpdate()
{
	if (!ENGINE.SELF->bIsPlaying) return;

	auto* p_em = ENGINE.ENT_MNGR;
	for (auto& entity : entities_)
	{
		auto& transformComp = p_em->GetComponent<Components::TransformComponent>(entity);
		auto& rigidbodyComp = p_em->GetComponent<Components::CapsuleRigidbodyComponent>(entity);

		if (transformComp.HasChanged())
		{
			rigidbodyComp.center = transformComp.position;
			rigidbodyComp.rotationParent = transformComp.rotation;
		}

		// Its faster to set the values rather than check if the Rigidbody has changed i would argue
		transformComp.position = rigidbodyComp.position - rigidbodyComp.centerOffset;
		transformComp.rotation = rigidbodyComp.rotationParent;
	}
}