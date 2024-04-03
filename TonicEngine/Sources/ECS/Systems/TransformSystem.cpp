#pragma once

#include "pch.hpp"

#include "ECS/Systems/TransformSystem.hpp"
#include "ECS/Components/Transform.hpp"

ECS::Systems::TransformSystem::TransformSystem()
{
	AddComponentSignature<Components::Transform>();
}

void ECS::Systems::TransformSystem::Update()
{
	//TODO: Create an entity Registry instead
	ECS::EntityManager* p_em = HEART::GetEntityManager();
	for (auto& entity : entities_)
	{
		if (p_em->HasChildren(entity))
		{
			Components::Transform& parentTr = p_em->GetComponent<Components::Transform>(entity);
			if (parentTr != lastValue)
			{
				for (auto& child : p_em->GetChildren(entity))
				{
					Components::Transform& childTr = p_em->GetComponent<Components::Transform>(child);
					childTr.position += parentTr->position - lastValue.poition;
					childTr.rotation += parentTr->rotation - lastValue.rotation;
					childTr.scale += parentTr->scale - lastValue.scale;
				}
			}
			parentTr->lastValue = *parentTr;
		}
	}
}