#pragma once

#include "pch.hpp"

#include "ECS/Systems/AudioListenerSystem.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Components/AudioListenerComponent.hpp"

ECS::Systems::AudioListenerSystem::AudioListenerSystem()
{
	AddComponentSignature<Components::TransformComponent>();
	AddComponentSignature<Components::AudioListenerComponent>();
}

void ECS::Systems::AudioListenerSystem::Update()
{
	ECS::EntityManager* p_em = ENGINE.ENT_MNGR;

	for (auto& entity : entities_)
	{
		Components::AudioListenerComponent& audioListener = p_em->GetComponent<Components::AudioListenerComponent>(entity);
		if (!audioListener.bEnabled)
			continue;
		Components::TransformComponent& transform = p_em->GetComponent<Components::TransformComponent>(entity);

		if (transform.HasChanged())
		{
			//TODO: Test here if the transform changed, or Event
			audioListener.SetPosition(transform.position);
			audioListener.SetDirection(transform.rotation);
		}

		//With Physics
		//audioListener.SetVelocity(rigidBody.Velocity);
	}
}