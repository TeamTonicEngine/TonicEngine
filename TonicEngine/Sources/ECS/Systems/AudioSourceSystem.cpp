#pragma once

#include "pch.hpp"

#include "ECS/Systems/AudioSourceSystem.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Components/AudioSourceComponent.hpp"

ECS::Systems::AudioSourceSystem::AudioSourceSystem()
{
	AddComponentSignature<Components::TransformComponent>();
	AddComponentSignature<Components::AudioSourceComponent>();
}

void ECS::Systems::AudioSourceSystem::Update()
{
	ECS::EntityManager* p_em = ENGINE.ENT_MNGR;

	for (auto& entity : entities_)
	{
		Components::AudioSourceComponent& audioSource = p_em->GetComponent<Components::AudioSourceComponent>(entity);
		if (!audioSource.bSpatialized_)
			continue;
		Components::TransformComponent& transform = p_em->GetComponent<Components::TransformComponent>(entity);
		//TODO: Test here if the transform changed, or Event
		if (transform.HasChanged())
		{
			audioSource.SetPosition(transform.position);
		}
		//With Physics
		//audioListener.SetVelocity(rigidBody.Velocity);
	}
}