#include "pch.hpp"
#include "ECS/Systems/ModelRendererSystem.hpp"
#include "ECS/Components/Transform.hpp"


ECS::Systems::ModelRendererSystem::ModelRendererSystem()
{
	AddComponentSignature<Components::Transform>();
	/////////////////////////////////////////////////////////////////////////
	//TODO: Uncomment on ModelRendererComponent Creation 
	//AddComponentSignature<Components::ModelRenderer>();
	/////////////////////////////////////////////////////////////////////////

	p_shader_ = HEART::GetResourceManager()->Get<Resources::Shader>("MESH");
}

void ECS::Systems::ModelRendererSystem::Render()
{
	EntityManager* p_em = HEART::GetEntityManager();
	Core::Renderer::RHI* p_rhi = HEART::GetRenderer();

	p_shader_->Use();
	

	for (auto entity : entities_)
	{
		p_em->GetComponent<Components::Transform>(entity).SetModelUniform();
	/////////////////////////////////////////////////////////////////////////
	//TODO: Uncomment on ModelRendererComponent Creation 
	//	p_em->GetComponent<Components::ModelRenderer>(entity).p_Renderer->Render(p_shader_);
	/////////////////////////////////////////////////////////////////////////

	}
}
