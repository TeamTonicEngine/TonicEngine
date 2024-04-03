#pragma once

#include "pch.hpp"

#include "ECS/Systems/SpotLightSystem.hpp"
#include "ECS/Components/SpotLightComponent.hpp"
#include "ECS/Components/Transform.hpp"
#include "Resources/Shader.hpp"


ECS::Systems::SpotLightSystem::SpotLightSystem()
{
	AddComponentSignature<Components::SpotLightComponent>();
}
void ECS::Systems::SpotLightSystem::Init()
{
	p_shader_ = HEART::GetResourceManager()->Get<Resources::Shader>("pbr");
}
void ECS::Systems::SpotLightSystem::Update()
{
	p_shader_->Use();
	Core::Renderer::RHI* p_rhi = HEART::GetRenderer();
	ECS::EntityManager* p_em = HEART::GetEntityManager();
	p_rhi->SetUniform("u_spotlightCount", (int)entities_.size());

	u32 index = 0;
	for (auto& entity : entities_)
	{
		auto& light = p_em->GetComponent<Components::SpotLightComponent>(entity);
		auto& transform = p_em->GetComponent<Components::Transform>(entity);
		
		p_rhi->SetUniform(std::string("u_spotlights[" + index) + "]"+".position", transform.position);
		p_rhi->SetUniform(std::string("u_spotlights[" + index) + "]" + ".color", light.p_light_->color_);
		p_rhi->SetUniform(std::string("u_spotlights[" + index) + "]" + ".strength", light.p_light_->strength_);
		p_rhi->SetUniform(std::string("u_spotlights[" + index) + "]" + ".shadow", light.p_light_->bShadow_);
		p_rhi->SetUniform(std::string("u_spotlights[" + index) + "]" + ".iCutOff", light.p_light_->inCutOff_);
		p_rhi->SetUniform(std::string("u_spotlights[" + index) + "]" + ".oCutOff", light.p_light_->outCutOff_);
		if(light.p_light_->bShadow_)
			p_rhi->SetUniform(std::string("u_spotlights[" + index) + "]" + ".bias", light.p_light_->bias_);

		++index;
	}
}