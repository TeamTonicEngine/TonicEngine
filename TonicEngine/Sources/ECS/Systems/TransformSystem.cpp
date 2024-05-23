#include "pch.hpp"

#include "ECS/Systems/TransformSystem.hpp"
#include "ECS/Components/TransformComponent.hpp"

ECS::Systems::TransformSystem::TransformSystem()
{
	AddComponentSignature<ECS::Components::TransformComponent>();
}

const bool ECS::Systems::TransformSystem::Init()
{
	bool success = true;
	if (success)
		DEBUG_SUCCESS("INITIALIZED Transform System")
	else
		throw ("Transform System failed to initialised");

	return success;
}

void ECS::Systems::TransformSystem::Update()
{
	ECS::EntityManager* p_em = ENGINE.ENT_MNGR;
	for (EntityID entity : p_em->GetEntityChildren(ROOT_ENTITY_ID))
	{
		ECS::Components::TransformComponent* parentTr = nullptr;
		if (p_em->HasComponent<ECS::Components::TransformComponent>(entity))
			parentTr = &p_em->GetComponent<ECS::Components::TransformComponent>(entity);

		RecursiveUpdate(entity, parentTr);
	}
	for (auto& entity : changedEntities_)
	{
		if (p_em->HasComponent<ECS::Components::TransformComponent>(entity))
			p_em->GetComponent<ECS::Components::TransformComponent>(entity).bChanged_ = true;
	}
}

void ECS::Systems::TransformSystem::RecursiveUpdate(EntityID _entity, Components::TransformComponent* _p_transform)
{
	bool bChanged = _p_transform && _p_transform->HasChanged();
	if (bChanged)
		changedEntities_.insert(_entity);

	ECS::EntityManager* p_em = ENGINE.ENT_MNGR;
	for (auto& child : p_em->GetEntityChildren(_entity))
	{
		//No transform means keep transform & go deeper
		if (!p_em->HasComponent<ECS::Components::TransformComponent>(child))
		{
			RecursiveUpdate(child, _p_transform);
			continue;
		}
		ECS::Components::TransformComponent& childTr = p_em->GetComponent<ECS::Components::TransformComponent>(child);
		if (bChanged)
		{
			if (_p_transform->positionDiff_ != 0)
				childTr.SetPosition(childTr.position - _p_transform->positionDiff_);
			if (_p_transform->rotationDiff_.xyzw != Maths::Quat::Identity().xyzw)
				childTr.SetRotation(childTr.rotation * _p_transform->rotationDiff_);
			if (_p_transform->scaleDiff_ != 1.f)
				childTr.SetScale(Maths::Vec3(childTr.scale.x * _p_transform->scaleDiff_.x, childTr.scale.y * _p_transform->scaleDiff_.y, childTr.scale.z * _p_transform->scaleDiff_.z));
		}
		RecursiveUpdate(child, &childTr);
	}

	//Reset Flags
	if (bChanged)
	{
		_p_transform->bChanged_ = false;
	}
}

void ECS::Systems::TransformSystem::LateUpdate()
{
	if (!changedEntities_.size())
		return;

	ECS::EntityManager* p_em = ENGINE.ENT_MNGR;
	for (auto& entity : changedEntities_)
	{
		//TODO: Think about what happens if the transform is changed(like in Editor) between Update and LateUpdate
		if (p_em->HasComponent<ECS::Components::TransformComponent>(entity))
			p_em->GetComponent<ECS::Components::TransformComponent>(entity).ResetChanged();
	}
	changedEntities_.clear();
}