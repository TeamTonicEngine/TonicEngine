#include "pch.hpp"

#include "ECS/Systems/SphereRigidbodySystem.hpp"

#include <ECS/Components/TransformComponent.hpp>
#include <ECS/Components/SphereRigidbodyComponent.hpp>

ECS::Systems::SphereRigidbodySystem::SphereRigidbodySystem()
{
	AddComponentSignature<Components::TransformComponent>();
	AddComponentSignature<Components::SphereRigidbodyComponent>();
}

void ECS::Systems::SphereRigidbodySystem::AddEntity(const EntityID _entity)
{
	entities_.insert(_entity);

	auto* p_em = ENGINE.ENT_MNGR;
	auto& transformComp = p_em->GetComponent<Components::TransformComponent>(_entity);
	auto& rigidbodyComp = p_em->GetComponent<Components::SphereRigidbodyComponent>(_entity);

	rigidbodyComp.Init(transformComp.scale, transformComp.position, transformComp.rotation);
}

void ECS::Systems::SphereRigidbodySystem::RemoveEntity(const EntityID _entity)
{
	entities_.erase(_entity);
	auto* p_em = ENGINE.ENT_MNGR;
	if (p_em->HasComponent<Components::SphereRigidbodyComponent>(_entity))
		p_em->GetComponent<Components::SphereRigidbodyComponent>(_entity).DestroyRigidbody();
}

void ECS::Systems::SphereRigidbodySystem::Update()
{
	if (ENGINE.SELF->bIsPlaying) return;

	auto* p_em = ENGINE.ENT_MNGR;
	for (auto& entity : entities_)
	{
		auto& transformComp = p_em->GetComponent<Components::TransformComponent>(entity);
		auto& rigidbodyComp = p_em->GetComponent<Components::SphereRigidbodyComponent>(entity);

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

void ECS::Systems::SphereRigidbodySystem::LateUpdate()
{
	if (!ENGINE.SELF->bIsPlaying) return;

	auto* p_em = ENGINE.ENT_MNGR;
	for (auto& entity : entities_)
	{
		auto& transformComp = p_em->GetComponent<Components::TransformComponent>(entity);
		auto& rigidbodyComp = p_em->GetComponent<Components::SphereRigidbodyComponent>(entity);

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