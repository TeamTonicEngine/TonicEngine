#include "pch.hpp"

#include "ECS/Systems/SpotLightSystem.hpp"
#include "ECS/Components/SpotLightComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "Resources/Shader.hpp"

ECS::Systems::SpotLightSystem::SpotLightSystem()
{
	AddComponentSignature<Components::SpotLightComponent>();
	AddComponentSignature<Components::TransformComponent>();
}

const bool ECS::Systems::SpotLightSystem::Init()
{
	//Pbr version
	p_shaders_.push_back(ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\pbr.frag")); //"MESH"
	//Phong version
	p_shaders_.push_back(ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\phong.frag")); //"MESH"

	bool success = true;
	for (auto shader : p_shaders_)
		success &= (shader != nullptr);
	if (success)
		DEBUG_SUCCESS("Initialized Spot Light System")
	else
		DEBUG_ERROR("Failed to initialize Spot Light System");

	return(success);
}

void ECS::Systems::SpotLightSystem::Render()
{
	if (!p_shaders_.size())
	{
		DEBUG_WARNING("No Shader to renderer Spot Lights");
		return;
	}

	Core::Renderer::RHI* p_rhi = ENGINE.RDR;
	ECS::EntityManager* p_em = ENGINE.ENT_MNGR;
	for (auto shader : p_shaders_)
	{
		shader->Use();

		p_rhi->SetSpotLightNumber((u32)entities_.size());
		u32 index = 0;
		for (auto& entity : entities_)
		{
			auto& lightComp = p_em->GetComponent<Components::SpotLightComponent>(entity);
			if (!lightComp.bEnabled)
			{
				auto light = lightComp.light;
				light.color = TNCColors::BLACK;
				p_rhi->SetLight(&light, index, Maths::Vec3{ 0 },Maths::Quat::Identity());
				++index;
				continue;
			}
			auto& light = lightComp.light;
			auto* transform = &p_em->GetComponent<Components::TransformComponent>(entity);

			p_rhi->SetLight(&light, index, transform->position, transform->rotation);

			++index;
		}
	}
}

void ECS::Systems::SpotLightSystem::RenderEditorScene() { Render(); }