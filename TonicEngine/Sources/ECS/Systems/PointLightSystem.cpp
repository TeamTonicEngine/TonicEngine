#pragma once

#include "pch.hpp"

#include "ECS/Systems/PointLightSystem.hpp"
#include "ECS/Components/PointLightComponent.hpp"
#include "ECS/Components/Transform.hpp"
#include "Resources/Shader.hpp"

ECS::Systems::PointLightSystem::PointLightSystem()
{
	AddComponentSignature<Components::PointLightComponent>();
}
void ECS::Systems::PointLightSystem::Init()
{
	p_shader_ = HEART::GetResourceManager()->Get<Resources::Shader>("pbr");
}
void ECS::Systems::PointLightSystem::Update()
{
	p_shader_->Use();
	Core::Renderer::RHI* p_rhi = HEART::GetRenderer();
	ECS::EntityManager* p_em = HEART::GetEntityManager();
	p_rhi->SetUniform("u_pointlightCount", (int)entities_.size());

	u32 index = 0;
	for (auto& entity : entities_)
	{
		auto& light = p_em->GetComponent<Components::PointLightComponent>(entity);
		auto& transform = p_em->GetComponent<Components::Transform>(entity);
		
		p_rhi->SetUniform(std::string("u_pointlights[" + index) + "]"+".position", transform.position);
		p_rhi->SetUniform(std::string("u_pointlights[" + index) + "]" + ".color", light.p_light_->color_);
		p_rhi->SetUniform(std::string("u_pointlights[" + index) + "]" + ".strength", light.p_light_->strength_);
		p_rhi->SetUniform(std::string("u_pointlights[" + index) + "]" + ".shadow", light.p_light_->bShadow_);
		if(light.p_light_->bShadow_)
			p_rhi->SetUniform(std::string("u_pointlights[" + index) + "]" + ".bias", light.p_light_->bias_);

		++index;
	}
}