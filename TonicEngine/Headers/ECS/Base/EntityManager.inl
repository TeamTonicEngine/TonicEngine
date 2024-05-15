#include "ECS/Base/EntityManager.hpp"

namespace ECS
{
	template<typename T, typename ... Args>
	T& EntityManager::AddComponent(const EntityID _entity, Args&&... _args)
	{
		if (_entity + 1 > MAX_ENTITY_COUNT)
			DEBUG_WARNING("EntityID out of range !  Component can't be created as entity nb%u doesn't exist.", (size_t)_entity);
		if (entities_.at(_entity)->components_.size() + 1 > MAX_COMP_COUNT)
			DEBUG_WARNING("Component count limit (%u) reached!", MAX_COMP_COUNT);
		T component(std::forward<Args>(_args)...);
		component.entityID_ = _entity;
		GetCompList<T>()->Insert(component);
		const ComponentTypeID compType = CompType<T>();
		GetEntitySignature(_entity)->insert(compType);
		UpdateEntityTargetSystems(_entity);

		return GetCompList<T>()->Get(_entity);
	}

	template<typename T>
	T& EntityManager::AddComponent(const EntityID _entity, T& _component)
	{
		if (_entity + 1 > MAX_ENTITY_COUNT)
			DEBUG_WARNING("EntityID out of range !  Component can't be created as entity nb%u doesn't exist.", (size_t)_entity);
		if (entities_.at(_entity)->components_.size() + 1 > MAX_COMP_COUNT)
			DEBUG_WARNING("Component count limit (%u) reached!", MAX_COMP_COUNT);
		_component.entityID_ = _entity;
		GetCompList<T>()->Insert(_component);
		const ComponentTypeID compType = CompType<T>();
		GetEntitySignature(_entity)->insert(compType);

		UpdateEntityTargetSystems(_entity);

		return GetCompList<T>()->Get(_entity);
	}

	template<typename T>
	void EntityManager::RemoveComponent(const EntityID _entity)
	{
		if (_entity + 1 > MAX_ENTITY_COUNT)
		{
			DEBUG_WARNING("EntityID out of range ! Component can't be destroyed as entity nb%u doesn't exist.", (size_t)_entity);
			return;
		}
		GetEntitySignature(_entity)->erase(CompType<T>());
		GetCompList<T>()->Erase(_entity);

		UpdateEntityTargetSystems(_entity);
	}

	template<typename T>
	T& EntityManager::GetComponent(const EntityID _entity)
	{
		if (_entity + 1 > MAX_ENTITY_COUNT)
			DEBUG_WARNING("EntityID out of range ! Component can't be accessed as entity nb%u doesn't exist.", (size_t)_entity);
		return GetCompList<T>()->Get(_entity);
	}

	template<typename T>
	const bool EntityManager::HasComponent(const EntityID _entity) const
	{
		//Kinda breaking the norm here but Is has no sense here
		if (_entity + 1 > MAX_ENTITY_COUNT)
		{
			DEBUG_WARNING("EntityID out of range ! Entity nb%u can't be destroyed as it doesn't exist.", (size_t)_entity);
			return false;
		}
		const EntitySignature* p_es = GetEntitySignature(_entity);
		const ComponentTypeID compType = CompType<T>();
		auto it = std::find(p_es->begin(),p_es->end(), compType);

		return (it != p_es->end());
	}

	template<typename T>
	//ALWAYS Register the TransformSystem 1st
	const bool EntityManager::RegisterSystem()
	{
		const SystemTypeID systemType = SystemType<T>();
		if (registeredSystems_.count(systemType) != 0)
			DEBUG_WARNING("System already registered!");
		std::unique_ptr<BaseSystem> system = std::make_unique<T>();

		//Add entities to new system
		for (EntityID entity = 0; entity < entityCount_; entity++)
			AddEntityToSystem(entity, system.get());

		bool success = system->Init();
		registeredSystems_[systemType] = std::move(system);
		return success;
	}

	template<typename T>
	void EntityManager::UnRegisterSystem()
	{
		const SystemTypeID systemType = SystemType<T>();
		if (registeredSystems_.count(systemType) == 0)
			DEBUG_WARNING("No system registered!");
		registeredSystems_.find(systemType)->Destroy();
		registeredSystems_.erase(systemType);
	}

	template<typename T>
	void EntityManager::AddCompList()
	{
		const ComponentTypeID compType = CompType<T>();
		if (componentsArrays_.find(compType) != componentsArrays_.end())
		{
			DEBUG_WARNING("Component list already registered!");
			return;
		}
		componentsArrays_[compType] = std::move(std::make_shared<CompList<T>>());
	}

	template<typename T>
	std::shared_ptr<CompList<T>> EntityManager::GetCompList()
	{
		const ComponentTypeID compType = CompType<T>();
		if (componentsArrays_.count(compType) == 0)
			AddCompList<T>();
		return std::static_pointer_cast<CompList<T>>(componentsArrays_.at(compType));
	}
}