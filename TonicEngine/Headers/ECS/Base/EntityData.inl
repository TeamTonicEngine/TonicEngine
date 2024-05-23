#include "ECS/Base/EntityData.hpp"
#include "EntityManager.hpp"

namespace ECS
{
	inline TONIC_ENGINE_API EntityData::EntityData(const EntityID _id, EntityManager* _p_manager)
		: id_(_id), p_mgr_(_p_manager), parent_(ROOT_ENTITY_ID), name_("Entity" + std::to_string(_id)) {
		if (_id != ROOT_ENTITY_ID)
			p_mgr_->ParentEntityToRoot(_id);
		else
			parent_ = INVALID_ENTITY_ID;
	}
	inline TONIC_ENGINE_API EntityData::EntityData(const EntityID _id, EntityManager* _p_manager, EntityID _parent)
		: id_(_id), p_mgr_(_p_manager), parent_(_parent), name_("Entity" + std::to_string(_id)) {
		p_mgr_->AddChild(_parent, _id);
	}
	inline const EntityID EntityData::GetID() const { return id_; }

	inline void EntityData::Destroy() { p_mgr_->DestroyEntity(id_); }

	inline void EntityData::AddChild(EntityID _entity) { p_mgr_->AddChild(id_, _entity); }

	inline void EntityData::RemoveChild(EntityID _entity) { p_mgr_->RemoveChild(id_, _entity); }

	template<typename T, typename... Args>
	inline void EntityData::AddComponent(Args&&..._args) { p_mgr_->AddComponent<T>(id_, std::forward<Args>(_args)...); }

	template<typename T>
	inline void EntityData::AddComponent(T& _component) { p_mgr_->AddComponent<T>(id_, _component); }

	template<typename T>
	inline T& EntityData::GetComponent() { return p_mgr_->GetComponent<T>(id_); }

	template<typename T>
	inline void EntityData::RemoveComponent() { p_mgr_->RemoveComponent<T>(id_); }

	template<typename T>
	inline const bool EntityData::HasComponent() { return p_mgr_->HasComponent<T>(id_); }
}

#include "ECS/Components.hpp"

// Explicit template instantiation for AudioListenerComponent
template TONIC_ENGINE_API ECS::Components::AudioListenerComponent& ECS::EntityData::GetComponent<ECS::Components::AudioListenerComponent>();
// Explicit template instantiation for AudioSourceComponent
template TONIC_ENGINE_API ECS::Components::AudioSourceComponent& ECS::EntityData::GetComponent<ECS::Components::AudioSourceComponent>();
// Explicit template instantiation for CameraComponent
template TONIC_ENGINE_API ECS::Components::CameraComponent& ECS::EntityData::GetComponent<ECS::Components::CameraComponent>();
// Explicit template instantiation for CppScriptComponent
template TONIC_ENGINE_API ECS::Components::CppScriptComponent& ECS::EntityData::GetComponent<ECS::Components::CppScriptComponent>();
// Explicit template instantiation for DirectionalLightComponent
template TONIC_ENGINE_API ECS::Components::DirectionalLightComponent& ECS::EntityData::GetComponent<ECS::Components::DirectionalLightComponent>();
// Explicit template instantiation for PointLightComponent
template TONIC_ENGINE_API ECS::Components::PointLightComponent& ECS::EntityData::GetComponent<ECS::Components::PointLightComponent>();
// Explicit template instantiation for SpotLightComponent
template TONIC_ENGINE_API ECS::Components::SpotLightComponent& ECS::EntityData::GetComponent<ECS::Components::SpotLightComponent>();
// Explicit template instantiation for MeshRendererComponent
template TONIC_ENGINE_API ECS::Components::MeshRendererComponent& ECS::EntityData::GetComponent<ECS::Components::MeshRendererComponent>();
// Explicit template instantiation for TextRendererComponent
template TONIC_ENGINE_API ECS::Components::TextRendererComponent& ECS::EntityData::GetComponent<ECS::Components::TextRendererComponent>();
// Explicit template instantiation for TransformComponent
template TONIC_ENGINE_API ECS::Components::TransformComponent& ECS::EntityData::GetComponent<ECS::Components::TransformComponent>();

// Explicit template instantiation for AudioListenerComponent
template TONIC_ENGINE_API const bool ECS::EntityData::HasComponent<ECS::Components::AudioListenerComponent>();
// Explicit template instantiation for AudioSourceComponent
template TONIC_ENGINE_API const bool ECS::EntityData::HasComponent<ECS::Components::AudioSourceComponent>();
// Explicit template instantiation for CameraComponent
template TONIC_ENGINE_API const bool ECS::EntityData::HasComponent<ECS::Components::CameraComponent>();
// Explicit template instantiation for CppScriptComponent
template TONIC_ENGINE_API const bool ECS::EntityData::HasComponent<ECS::Components::CppScriptComponent>();
// Explicit template instantiation for DirectionalLightComponent
template TONIC_ENGINE_API const bool ECS::EntityData::HasComponent<ECS::Components::DirectionalLightComponent>();
// Explicit template instantiation for PointLightComponent
template TONIC_ENGINE_API const bool ECS::EntityData::HasComponent<ECS::Components::PointLightComponent>();
// Explicit template instantiation for SpotLightComponent
template TONIC_ENGINE_API const bool ECS::EntityData::HasComponent<ECS::Components::SpotLightComponent>();
// Explicit template instantiation for MeshRendererComponent
template TONIC_ENGINE_API const bool ECS::EntityData::HasComponent<ECS::Components::MeshRendererComponent>();
// Explicit template instantiation for TextRendererComponent
template TONIC_ENGINE_API const bool ECS::EntityData::HasComponent<ECS::Components::TextRendererComponent>();
// Explicit template instantiation for TransformComponent
template TONIC_ENGINE_API const bool ECS::EntityData::HasComponent<ECS::Components::TransformComponent>();