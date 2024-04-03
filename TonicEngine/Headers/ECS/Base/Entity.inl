#include "ECS/Base/Entity.hpp"
#include "EntityManager.hpp"

namespace ECS
{
	inline const EntityID Entity::GetID() const { return id_; }

	inline void Entity::Destroy() { p_mgr_->DestroyEntity(id_); }

	inline void Entity::AddChild(EntityID _entity)
	{
		auto it = std::find_if(children_.cbegin(), children_.cend(), [&](EntityID entity) {return entity == _entity; });
		if (_entity != id_ && it != children_.cend())
			children_.push_back(_entity);
	}

	inline void Entity::RemoveChild(EntityID _entity)
	{
		auto it = std::find_if(children_.cbegin(), children_.cend(), [&](EntityID entity) {return entity == _entity; });
		if (it != children_.cend())
			children_.erase(it);
	}

	template<typename T, typename... Args>
	inline void Entity::AddComponent(Args&&..._args) { p_mgr_->AddComponent<T>(id_, std::forward<Args>(_args)...); }

	template<typename T>
	inline void Entity::AddComponent(T& _component) { p_mgr_->AddComponent<T>(id_, _component); }

	template<typename T>
	inline T& Entity::GetComponent() { return p_mgr_->GetComponent<T>(id_); }

	template<typename T>
	inline void Entity::RemoveComponent() { p_mgr_->RemoveComponent<T>(id_); }

	template<typename T>
	inline const bool Entity::HasComponent() { return p_mgr_->HasComponent<T>(id_); }
}