#include "pch.hpp"

#include "ECS/Base/BaseSystem.hpp"

void ECS::BaseSystem::AddEntity(const EntityID _entity) { entities_.insert(_entity); }

void ECS::BaseSystem::RemoveEntity(const EntityID _entity) { entities_.erase(_entity); }

const ECS::EntitySignature ECS::BaseSystem::GetSignature() const { return signature_; }

void ECS::BaseSystem::Start() {}

void ECS::BaseSystem::Update()
{
	for (auto i : entities_)
		std::cout << i << " ";
	std::cout << std::endl;
}

void ECS::BaseSystem::Render() {}

void ECS::BaseSystem::Destroy() {}