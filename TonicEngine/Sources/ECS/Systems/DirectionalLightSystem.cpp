#pragma once

#include "pch.hpp"

#include "ECS/Systems/DirectionalLightSystem.hpp"
#include "ECS/Components/DirectionalLightComponent.hpp"
#include "ECS/Components/Transform.hpp"
#include "Resources/Shader.hpp"


ECS::Systems::DirectionalLightSystem::DirectionalLightSystem()
{
	AddComponentSignature<Components::DirectionalLightComponent>();
}
void ECS::Systems::DirectionalLightSystem::Init()
{
	p_shader_ = HEART::GetResourceManager()->Get<Resources::Shader>("pbr");
}
void ECS::Systems::DirectionalLightSystem::Update()
{
	p_shader_->Use();
	Core::Renderer::RHI* p_rhi = HEART::GetRenderer();
	ECS::EntityManager* p_em = HEART::GetEntityManager();
	p_rhi->SetUniform("u_directlightCount", (int)entities_.size());

	u32 index = 0;
	for (auto& entity : entities_)
	{
		auto& light = p_em->GetComponent<Components::DirectionalLightComponent>(entity);
		auto& transform = p_em->GetComponent<Components::Transform>(entity);
		
		p_rhi->SetUniform(std::string("u_directlights[" + index) + "]"+".direction", transform.position);
		p_rhi->SetUniform(std::string("u_directlights[" + index) + "]" + ".color", light.p_light_->color_);
		p_rhi->SetUniform(std::string("u_directlights[" + index) + "]" + ".strength", light.p_light_->strength_);
		p_rhi->SetUniform(std::string("u_directlights[" + index) + "]" + ".shadow", light.p_light_->bShadow_);
		if(light.p_light_->bShadow_)
			p_rhi->SetUniform(std::string("u_directlights[" + index) + "]" + ".bias", light.p_light_->bias_);

		++index;
	}
}