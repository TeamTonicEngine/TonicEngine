#include "pch.hpp"

#include "Core/Log.hpp"
#include "ECS/Base/EntityManager.hpp"
#include "ECS/Base/Entity.hpp"


ECS::EntityManager::EntityManager() : entityCount_(0)
{
	for (EntityID entity = 0; entity < MAX_ENTITY_COUNT; entity++)
		availableEntities_.push(entity);
}

void ECS::EntityManager::Update()
{
	for (auto& system : registeredSystems_)
		system.second->Update();
}

void ECS::EntityManager::Render()
{
	for (auto& system : registeredSystems_)
		system.second->Render();
}

const ECS::EntityID ECS::EntityManager::AddNewEntity()
{
	const EntityID entityID = availableEntities_.front();
	AddEntitySignature(entityID);
	availableEntities_.pop();
	entityCount_++;
	return entityID;
}

void ECS::EntityManager::DestroyEntity(const EntityID _entity)
{
	if (_entity + 1 > MAX_ENTITY_COUNT)
	{
		DEBUG_WARNING("EntityID out of range ! Entity nb%u can't be destroyed as it doesn't exist.", (size_t)_entity);
		return;
	}

	//Remove in referencement
	entitiesSignatures_.erase(_entity);
	for (auto& components : componentsArrays_)
		components.second->Erase(_entity);
	for (auto& system : registeredSystems_)
		system.second->RemoveEntity(_entity);
	//Update the used/avalaible entities
	entityCount_--;
	availableEntities_.push(_entity);
}

void ECS::EntityManager::AddEntitySignature(const EntityID _entity)
{
	if (entitiesSignatures_.find(_entity) != entitiesSignatures_.end())
	{
		DEBUG_WARNING("Signature not found!");
		return;
	}
	entitiesSignatures_[_entity] = std::move(std::make_shared <EntitySignature>());
}

std::shared_ptr<ECS::EntitySignature> ECS::EntityManager::GetEntitySignature(const EntityID _entity)
{
	if (entitiesSignatures_.find(_entity) == entitiesSignatures_.end())
	{
		DEBUG_WARNING("Signature not found!");
		return nullptr;
	}
	return entitiesSignatures_.at(_entity);
}

void ECS::EntityManager::UpdateEntityTargetSystems(const EntityID _entity)
{
	for (auto& system : registeredSystems_)
		AddEntityToSystem(_entity, system.second.get());
}

void ECS::EntityManager::AddEntityToSystem(const EntityID _entity, BaseSystem* _p_system)
{
	if (BelongToSystem(_entity, _p_system->signature_))
		_p_system->entities_.insert(_entity);
	else
		_p_system->entities_.erase(_entity);
}

bool ECS::EntityManager::BelongToSystem(const EntityID _entity, const EntitySignature& _systemSignature)
{
	for (const auto& compType : _systemSignature)
		if (GetEntitySignature(_entity)->count(compType) == 0)
			return false;

	return true;
}