#pragma once
#include "DLL_API.hpp"

#include <queue>
#include <map>
#include <memory>
#include <cassert>

#include "BaseComponent.hpp"
#include "BaseSystem.hpp"
#include "CompList.hpp"
#include "Types.hpp"

namespace ECS
{
	class EntityManager
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		EntityID entityCount_;
		std::queue<EntityID> availableEntities_;
		std::map<EntityID, std::shared_ptr<EntitySignature>> entitiesSignatures_;
		std::map<SystemTypeID, std::shared_ptr<BaseSystem>> registeredSystems_;
		std::map<ComponentTypeID, std::shared_ptr<ICompList>> componentsArrays_;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		TONIC_ENGINE_API EntityManager();
		TONIC_ENGINE_API ~EntityManager() = default;

		TONIC_ENGINE_API void Update();

		TONIC_ENGINE_API void Render();

		TONIC_ENGINE_API const EntityID AddNewEntity();

		TONIC_ENGINE_API void DestroyEntity(const EntityID _entity);

		template<typename T, typename... Args>
		void AddComponent(const EntityID _entity, Args&&... _args)
			/* Function body is in this hpp file because of template */;

		template<typename T>
		void AddComponent(const EntityID _entity, T& _component)
			/* Function body is in this hpp file because of template */;

		template<typename T>
		void RemoveComponent(const EntityID _entity)
			/* Function body is in this hpp file because of template */;

		template<typename T>
		T& GetComponent(const EntityID _entity)
			/* Function body is in this hpp file because of template */;

		template<typename T>
		const bool HasComponent(const EntityID _entity)
			/* Function body is in this hpp file because of template */;

		template<typename T>
		void RegisterSystem()
			/* Function body is in this hpp file because of template */;

		template<typename T>
		void UnRegisterSystem()
			/* Function body is in this hpp file because of template */;

	private:
		template<typename T>
		void AddCompList()
			/* Function body is in this hpp file because of template */;

		template<typename T>
		std::shared_ptr<CompList<T>> GetCompList()
			/* Function body is in this hpp file because of template */;

		TONIC_ENGINE_API void AddEntitySignature(const EntityID _entity);

		TONIC_ENGINE_API std::shared_ptr<EntitySignature> GetEntitySignature(const EntityID _entity);

		TONIC_ENGINE_API void UpdateEntityTargetSystems(const EntityID _entity);

		TONIC_ENGINE_API void AddEntityToSystem(const EntityID _entity, BaseSystem* _p_system);

		TONIC_ENGINE_API bool BelongToSystem(const EntityID _entity, const EntitySignature& _systemSignature);
	};

#pragma region EntityManagerTemplateFunctionsDefinition
	/*
	template<typename T, typename ... Args>
	void EntityManager::AddComponent(const EntityID _entity, Args&&... _args)
	{
		if (_entity + 1 > MAX_ENTITY_COUNT)
		{
			DEBUG_WARNING("EntityID out of range !  Component can't be created as entity nb%u doesn't exist.", (size_t)_entity);
			return;
		}
		if (entitiesSignatures_.size() + 1 > MAX_COMP_COUNT)
		{
			DEBUG_WARNING("Component count limit (%u) reached!", MAX_COMP_COUNT);
			return;
		}
		T component(std::forward<Args>(_args)...);
		component.entityID_ = _entity;
		GetEntitySignature(_entity)->insert(CompType<T>());
		GetCompList<T>()->Insert(component);

		UpdateEntityTargetSystems(_entity);
	}

	template<typename T>
	void EntityManager::AddComponent(const EntityID _entity, T& _component)
	{
		if (_entity + 1 > MAX_ENTITY_COUNT)
		{
			DEBUG_WARNING("EntityID out of range !  Component can't be created as entity nb%u doesn't exist.", (size_t)_entity);
			return;
		}
		if (entitiesSignatures_.size() + 1 > MAX_COMP_COUNT)
		{
			DEBUG_WARNING("Component count limit (%u) reached!", MAX_COMP_COUNT);
			return;
		}
		_component.entityID_ = _entity;
		GetEntitySignature(_entity)->insert(_component);
		GetCompList<T>()->Insert(_component);

		UpdateEntityTargetSystems(_entity);
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
	const bool EntityManager::HasComponent(const EntityID _entity)
	{
		//Kinda breaking the norm here but Is has no sense here
		if (_entity + 1 > MAX_ENTITY_COUNT)
		{
			DEBUG_WARNING("EntityID out of range ! Entity nb%u can't be destroyed as it doesn't exist.", (size_t)_entity);
			return false;
		}
		return(GetEntitySignature(_entity)->count(CompType<T>()) > 0);
	}

	template<typename T>
	void EntityManager::RegisterSystem()
	{
		const SystemTypeID systemType = SystemType<T>();
		if (registeredSystems_.count(systemType) != 0)
			DEBUG_WARNING("System already registered!");
		auto system = std::make_shared<T>();

		//Add entities to new system
		for (EntityID entity = 0; entity < entityCount_; entity++)
			AddEntityToSystem(entity, system.get());

		system->Start();
		registeredSystems_[systemType] = std::move(system);
	}

	template<typename T>
	void EntityManager::UnRegisterSystem()
	{
		const SystemTypeID systemType = SystemType<T>();
		if (registeredSystems_.count(systemType) == 0)
			DEBUG_WARNING("No system registered!");
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
	}*/
#pragma endregion
}

#include "EntityManager.inl"