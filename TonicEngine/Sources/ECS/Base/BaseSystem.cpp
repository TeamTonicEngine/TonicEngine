#include "pch.hpp"

#include "ECS/Base/BaseSystem.hpp"

void ECS::BaseSystem::AddEntity(const EntityID _entity) 
{
	entities_.insert(_entity); 
}

void ECS::BaseSystem::RemoveEntity(const EntityID _entity) { entities_.erase(_entity); }

const ECS::EntitySignature ECS::BaseSystem::GetSignature() const { return signature_; }

const bool ECS::BaseSystem::Init()
{
	return true;
}

void ECS::BaseSystem::Update() {}

void ECS::BaseSystem::LateUpdate() {}

void ECS::BaseSystem::Render() {}

void ECS::BaseSystem::RenderEditorScene()
{
}

void ECS::BaseSystem::Destroy() { entities_.clear(); }