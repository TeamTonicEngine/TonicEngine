#pragma once

#include "EntityManager.hpp"


namespace ECS
{
	class EntityManager;

	class Entity
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		std::string name_;
		EntityID id_ = INVALID_ENTITY_ID;
		EntityManager* p_mgr_;
		EntityID parent_ = INVALID_ENTITY_ID;
		std::vector<EntityID> children_;
		EntitySignature signature_;

		/**********************************************
				FUNCTIONS BLOC
		**********************************************/
	public:
		TONIC_ENGINE_API Entity(const EntityID _id, EntityManager* _p_manager, EntityID _parent = INVALID_ENTITY_ID)
			: id_(_id), p_mgr_(_p_manager),parent_(_parent), name_("Entity"+_id) {}
		TONIC_ENGINE_API ~Entity() = default;

		TONIC_ENGINE_API const EntityID GetID() const;
		TONIC_ENGINE_API void Destroy();

		template<typename T, typename ... Args>
		void AddComponent(Args&& ... _args);

		template<typename T>
		void AddComponent(T& _component);

		template<typename T>
		T& GetComponent();

		template<typename T>
		void RemoveComponent();

		template<typename T>
		const bool HasComponent();

		void AddChild(EntityID _entity);

		void RemoveChild(EntityID _entity);

		const bool HasChildren() { return !children_.empty(); };

		__declspec(property(get = GetChildren))
			std::vector<EntityID> children;
		std::vector<EntityID> GetChildren() const { return children_; };

		EntityID GetChild(int _index) const { return children_.at(_index); };

		__declspec(property(get = GetParent))
			EntityID parent;
		EntityID GetParent() const { return parent_; };

		friend class EntityManager;
	};
}

#include "Entity.inl"