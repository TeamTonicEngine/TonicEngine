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
#include "EntityData.hpp"

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
		std::map<EntityID, std::shared_ptr<EntityData>> entities_;
		std::map<SystemTypeID, std::unique_ptr<BaseSystem>> registeredSystems_;
		std::map<ComponentTypeID, std::shared_ptr<ICompList>> componentsArrays_;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		TONIC_ENGINE_API EntityManager();
		bool TONIC_ENGINE_API Init();
		TONIC_ENGINE_API ~EntityManager();

		void TONIC_ENGINE_API Update();

		void TONIC_ENGINE_API LateUpdate();

		void TONIC_ENGINE_API Render();

		void TONIC_ENGINE_API RenderEditorScene();

		void TONIC_ENGINE_API Destroy();

		const TONIC_ENGINE_API EntityID AddNewEntity();
		const void TONIC_ENGINE_API AddEntityWithId(const EntityID _id);

		void TONIC_ENGINE_API DestroyEntity(const EntityID _entity);
		void TONIC_ENGINE_API DestroyAllEntitiesFromEntity(const EntityID _entity = 0);

		void TONIC_ENGINE_API ResetAvailableEntities();

		inline std::map<EntityID, std::shared_ptr<EntityData>> TONIC_ENGINE_API GetEntities() { return entities_; }

		template<typename T, typename... Args>
		T& AddComponent(const EntityID _entity, Args&&... _args)
			/* Function body is in this hpp file because of template */;

		template<typename T>
		T& AddComponent(const EntityID _entity, T& _component)
			/* Function body is in this hpp file because of template */;

		template<typename T>
		void RemoveComponent(const EntityID _entity)
			/* Function body is in this hpp file because of template */;

		template<typename T>
		T& GetComponent(const EntityID _entity)
			/* Function body is in this hpp file because of template */;

		template<typename T>
		const bool HasComponent(const EntityID _entity) const
			/* Function body is in this hpp file because of template */;

		template<typename T>
		//ALWAYS Register the TransformSystem 1st
		const bool RegisterSystem()
			/* Function body is in this hpp file because of template */;

		template<typename T>
		void UnRegisterSystem()
			/* Function body is in this hpp file because of template */;

#pragma region Hierarchy

		void TONIC_ENGINE_API AddChild(EntityID _parent, EntityID _child);
		void TONIC_ENGINE_API RemoveChild(EntityID _parent, EntityID _child);
		const bool TONIC_ENGINE_API HasChildren(EntityID _entity) const { return !entities_.at(_entity)->children_.empty(); };

		std::vector<EntityID> TONIC_ENGINE_API GetChildren(EntityID _entity) const { return entities_.at(_entity)->GetChildren(); };
		EntityID TONIC_ENGINE_API GetChild(EntityID _entity, int _index) const { return entities_.at(_entity)->GetChild(_index); };
		EntityID TONIC_ENGINE_API GetParent(EntityID _entity) const { return entities_.at(_entity)->GetParent(); };

		void TONIC_ENGINE_API ParentEntity(const EntityID _child, const EntityID _parent);

		void TONIC_ENGINE_API ParentEntityToRoot(const EntityID _entity);

		EntityID TONIC_ENGINE_API GetEntityParent(const EntityID& _entity) const;
		std::vector<EntityID> TONIC_ENGINE_API GetEntityChildren(const EntityID& _entity) const;
		EntityID TONIC_ENGINE_API GetEntityChild(const EntityID& _entity, size_t _childIdx = 0) const;

#pragma endregion //Hierarchy

		std::shared_ptr<EntityData> TONIC_ENGINE_API GetEntityData(const EntityID& _entity);

		template<typename T>
		std::shared_ptr<CompList<T>> GetCompList()
			/* Function body is in this hpp file because of template */;

	private:
		template<typename T>
		void AddCompList()
			/* Function body is in this hpp file because of template */;



		void TONIC_ENGINE_API AddEntityData(const EntityID _entity);

		TONIC_ENGINE_API EntitySignature* GetEntitySignature(const EntityID& _entity) const;

		void TONIC_ENGINE_API UpdateEntityTargetSystems(const EntityID _entity);

		void TONIC_ENGINE_API AddEntityToSystem(const EntityID _entity, BaseSystem* _p_system);

		const bool TONIC_ENGINE_API BelongToSystem(const EntityID& _entity, const EntitySignature& _systemSignature) const;
	};
}