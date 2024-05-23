#pragma once

#include "pch.hpp"

#include "ECS/Systems/DirectionalLightSystem.hpp"
#include "ECS/Components/DirectionalLightComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "Resources/Shader.hpp"

ECS::Systems::DirectionalLightSystem::DirectionalLightSystem()
{
	AddComponentSignature<Components::DirectionalLightComponent>();
	AddComponentSignature<Components::TransformComponent>();
}
const bool ECS::Systems::DirectionalLightSystem::Init()
{
	//Pbr version
	p_shaders_.push_back(ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\pbr.frag")); //"MESH"
	//Phong version
	p_shaders_.push_back(ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\phong.frag")); //"MESH"

	bool success = true;
	for (auto shader : p_shaders_)
		success &= (shader != nullptr);
	if (success)
		DEBUG_SUCCESS("Initialized Directional Light System")
	else
		DEBUG_ERROR("Failed to initialize Directional Light System");

	return(success);
}
void ECS::Systems::DirectionalLightSystem::Render()
{
	if (!p_shaders_.size())
	{
		DEBUG_WARNING("No Shader to renderer Directional Lights");
		return;
	}
	Core::Renderer::RHI* p_rhi = ENGINE.RDR;
	ECS::EntityManager* p_em = ENGINE.ENT_MNGR;
	for (auto shader : p_shaders_)
	{

		shader->Use();
		p_rhi->SetDirectionalLightNumber((u32)entities_.size());
		u32 index = 0;
		for (auto& entity : entities_)
		{
			auto& lightComp = p_em->GetComponent<Components::DirectionalLightComponent>(entity);
			if (!lightComp.bEnabled)
			{
				auto light = lightComp.light_;
				light.color = TNCColors::BLACK;
				p_rhi->SetLight(&light, index, {0});
				++index;
				continue;
			}
			auto& light = lightComp.light_;
			auto& transform = p_em->GetComponent<Components::TransformComponent>(entity);

			p_rhi->SetLight(&light, index, transform.position);

			++index;
		}
	}
}
void ECS::Systems::DirectionalLightSystem::RenderEditorScene()
{
	Render();
}