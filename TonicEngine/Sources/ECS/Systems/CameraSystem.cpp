#include "ECS/Systems/CameraSystem.hpp"
#include "Maths/Maths.hpp"
#include "Resources/Shader.hpp"

using namespace ECS::Systems;
CameraSystem::CameraSystem()
{
	AddComponentSignature<Components::CameraComponent>();
	AddComponentSignature<Components::Transform>();
	///////////////////////////////////////////////////////////////////////////////////////
	// TODO: Uncomment when Shader is done
	//p_skyboxShader_ = HEART::GetResourceManager()->Get("SKYBOX");
	//p_shaders_.push_back(HEART::GetResourceManager()->Get("MESH"));
	//p_shaders_.push_back(HEART::GetResourceManager()->Get("MODEL"));
	//p_shaders_.push_back(HEART::GetResourceManager()->Get("OUTLINE"));
	///////////////////////////////////////////////////////////////////////////////////////

	DEBUG_SUCCESS("Initialized Camera System");
}

void ECS::Systems::CameraSystem::Update()
{
	///////////////////////////////////////////////////////////////////////////////////////
	//Test Part
	alpha++;
	if (alpha > 360.f)
		alpha = 0.f;
	///////////////////////////////////////////////////////////////////////////////////////
	EntityManager* p_em = HEART::GetEntityManager();
	Core::Renderer::RHI* p_rhi = HEART::GetRenderer();
	for (auto entity : entities_)
	{
		using namespace Components;
		CameraComponent& cameraComponent = p_em->GetComponent<CameraComponent>(entity);
		LowRenderer::Camera* p_camera = cameraComponent.p_camera_;
		Transform& transform = p_em->GetComponent<Transform>(entity);
	
//Update view
		p_camera->SetView(transform.position, transform.rotation);
		///////////////////////////////////////////////////////////////////////////////////////
			//Test part
		transform.rotation = Maths::Quaternions::FromEulerAngles({ 0.f,alpha * Maths::Constants::DEG2RAD,0.f });
		///////////////////////////////////////////////////////////////////////////////////////

		p_skyboxShader_->Use();
		p_rhi->SetUniform("Projection", p_camera->projection);
		for (auto shader : p_shaders_)
		{
			shader->Use();
			p_rhi->SetUniform("CameraPos", transform.position);
			p_rhi->SetUniform("Projection", p_camera->projection);
			p_rhi->SetUniform("View", p_camera->view);

		}
	}
	
}
