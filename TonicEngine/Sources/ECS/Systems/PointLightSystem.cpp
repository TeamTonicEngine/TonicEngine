#pragma once

#include "pch.hpp"

#include "ECS/Systems/PointLightSystem.hpp"
#include "ECS/Components/PointLightComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "Resources/Shader.hpp"

ECS::Systems::PointLightSystem::PointLightSystem()
{
	AddComponentSignature<Components::PointLightComponent>();
	AddComponentSignature<Components::TransformComponent>();
}
const bool ECS::Systems::PointLightSystem::Init()
{
	//pbr Version
	p_shaders_.push_back(ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\pbr.frag")); //"MESH"
	//phong Version
	p_shaders_.push_back(ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\phong.frag")); //"MESH"

	bool success = true;
	for (auto shader : p_shaders_)
		success &= (shader != nullptr);
	if (success)
		DEBUG_SUCCESS("Initialized Point Light System")
	else
		DEBUG_ERROR("Failed to initialize Point Light System");

	return(success);
}
void ECS::Systems::PointLightSystem::Render()
{
	if (!p_shaders_.size())
	{
		DEBUG_WARNING("No Shader to renderer point Lights");
		return;
	}

	Core::Renderer::RHI* p_rhi = ENGINE.RDR;
	ECS::EntityManager* p_em = ENGINE.ENT_MNGR;
	for (auto shader : p_shaders_) {

		shader->Use();
		p_rhi->SetPointLightNumber((u32)entities_.size());

		u32 index = 0;
		for (auto& entity : entities_)
		{
			if (!p_em->HasComponent<Components::TransformComponent>(entity) || !p_em->HasComponent<Components::PointLightComponent>(entity))
				continue;
			auto& light = p_em->GetComponent<Components::PointLightComponent>(entity).light_;
			auto& transform = p_em->GetComponent<Components::TransformComponent>(entity);

			p_rhi->SetLight(&light, index, transform.position);

			++index;
		}
	}
}
void ECS::Systems::PointLightSystem::RenderEditorScene()
{
	Render();
}