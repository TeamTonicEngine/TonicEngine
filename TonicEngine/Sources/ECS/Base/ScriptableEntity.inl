#include "pch.hpp"
#include "ECS/Base/ScriptableEntity.hpp"
#include "ECS/Components.hpp"

ECS::ScriptableEntity::ScriptableEntity(EntityID _id) { p_entity_ = ENGINE.ENT_MNGR->GetEntityData(_id); }

template<typename T>
T& ECS::ScriptableEntity::GetComponent()
{
	return p_entity_->GetComponent<T>();
}
template<typename T>
bool ECS::ScriptableEntity::HasComponent()
{
	return p_entity_->HasComponent<T>();
}
#include "ECS/Components.hpp"

// Explicit template instantiation for AudioListenerComponent
template TONIC_ENGINE_API ECS::Components::AudioListenerComponent& ECS::ScriptableEntity::GetComponent<ECS::Components::AudioListenerComponent>();
// Explicit template instantiation for AudioSourceComponent
template TONIC_ENGINE_API ECS::Components::AudioSourceComponent& ECS::ScriptableEntity::GetComponent<ECS::Components::AudioSourceComponent>();
// Explicit template instantiation for CameraComponent
template TONIC_ENGINE_API ECS::Components::CameraComponent& ECS::ScriptableEntity::GetComponent<ECS::Components::CameraComponent>();
// Explicit template instantiation for CppScriptComponent
template TONIC_ENGINE_API ECS::Components::CppScriptComponent& ECS::ScriptableEntity::GetComponent<ECS::Components::CppScriptComponent>();
// Explicit template instantiation for DirectionalLightComponent
template TONIC_ENGINE_API ECS::Components::DirectionalLightComponent& ECS::ScriptableEntity::GetComponent<ECS::Components::DirectionalLightComponent>();
// Explicit template instantiation for PointLightComponent
template TONIC_ENGINE_API ECS::Components::PointLightComponent& ECS::ScriptableEntity::GetComponent<ECS::Components::PointLightComponent>();
// Explicit template instantiation for SpotLightComponent
template TONIC_ENGINE_API ECS::Components::SpotLightComponent& ECS::ScriptableEntity::GetComponent<ECS::Components::SpotLightComponent>();
// Explicit template instantiation for MeshRendererComponent
template TONIC_ENGINE_API ECS::Components::MeshRendererComponent& ECS::ScriptableEntity::GetComponent<ECS::Components::MeshRendererComponent>();
// Explicit template instantiation for TextRendererComponent
template TONIC_ENGINE_API ECS::Components::TextRendererComponent& ECS::ScriptableEntity::GetComponent<ECS::Components::TextRendererComponent>();
// Explicit template instantiation for TransformComponent
template TONIC_ENGINE_API ECS::Components::TransformComponent& ECS::ScriptableEntity::GetComponent<ECS::Components::TransformComponent>();

// Explicit template instantiation for AudioListenerComponent
template TONIC_ENGINE_API bool ECS::ScriptableEntity::HasComponent<ECS::Components::AudioListenerComponent>();
// Explicit template instantiation for AudioSourceComponent
template TONIC_ENGINE_API bool ECS::ScriptableEntity::HasComponent<ECS::Components::AudioSourceComponent>();
// Explicit template instantiation for CameraComponent
template TONIC_ENGINE_API bool ECS::ScriptableEntity::HasComponent<ECS::Components::CameraComponent>();
// Explicit template instantiation for CppScriptComponent
template TONIC_ENGINE_API bool ECS::ScriptableEntity::HasComponent<ECS::Components::CppScriptComponent>();
// Explicit template instantiation for DirectionalLightComponent
template TONIC_ENGINE_API bool ECS::ScriptableEntity::HasComponent<ECS::Components::DirectionalLightComponent>();
// Explicit template instantiation for PointLightComponent
template TONIC_ENGINE_API bool ECS::ScriptableEntity::HasComponent<ECS::Components::PointLightComponent>();
// Explicit template instantiation for SpotLightComponent
template TONIC_ENGINE_API bool ECS::ScriptableEntity::HasComponent<ECS::Components::SpotLightComponent>();
// Explicit template instantiation for MeshRendererComponent
template TONIC_ENGINE_API bool ECS::ScriptableEntity::HasComponent<ECS::Components::MeshRendererComponent>();
// Explicit template instantiation for TextRendererComponent
template TONIC_ENGINE_API bool ECS::ScriptableEntity::HasComponent<ECS::Components::TextRendererComponent>();
// Explicit template instantiation for TransformComponent
template TONIC_ENGINE_API bool ECS::ScriptableEntity::HasComponent<ECS::Components::TransformComponent>();