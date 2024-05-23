#include "pch.hpp"

#include "ECS/Systems/CameraSystem.hpp"
#include "Maths/Maths.hpp"
#include "Resources/Shader.hpp"

using namespace ECS::Systems;
CameraSystem::CameraSystem()
{
	AddComponentSignature<Components::TransformComponent>();
	AddComponentSignature<Components::CameraComponent>();
	p_skyboxShader_ = nullptr;
}

const bool CameraSystem::Init()
{
	bool success = true;
	///////////////////////////////////////////////////////////////////////////////////////
	p_skyboxShader_ = ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\skybox.frag");
	p_shaders_.push_back(ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\pbr.frag")); //"MESH"
	p_shaders_.push_back(ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\phong.frag")); //"MESH"
	p_shaders_.push_back(ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\unlit.frag")); //"MESH"
	p_shaders_.push_back(ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\text.frag")); //"Font"

	//p_shaders_.push_back(ENGINE.RES_MNGR->Get<Resources::Shader>("MODEL"));
	//p_shaders_.push_back(ENGINE.RES_MNGR->Get<Resources::Shader>("OUTLINE"));
	// success = p_skyboxShader_ && p_shaders_[0] && p_shaders_[1] && p_shaders_[2];
	///////////////////////////////////////////////////////////////////////////////////////

	success = p_shaders_[0].get();
	if (success)
		DEBUG_SUCCESS("Initialized Camera System")
	else
		DEBUG_ERROR("Failed to initialize Camera System");

	return success;
}

void CameraSystem::Destroy()
{
	p_shaders_.clear();
}
void ECS::Systems::CameraSystem::Update()
{
	EntityManager* p_em = ENGINE.ENT_MNGR;
	for (auto entity : entities_)
	{
		using namespace Components;
		TransformComponent& transform = p_em->GetComponent<TransformComponent>(entity);
			CameraComponent& cameraComponent = p_em->GetComponent<CameraComponent>(entity);
			if(!cameraComponent.bEnabled)
				continue;

			LowRenderer::Cameras::Camera* p_camera = &cameraComponent.camera_;
		if (transform.HasChanged())
		{

			//Update view
			p_camera->SetView(transform.position, transform.rotation);
		}
		p_camera->ComputeViewProjection();
	}
}
void ECS::Systems::CameraSystem::Render()
{
	EntityManager* p_em = ENGINE.ENT_MNGR;
	Core::Renderer::RHI* p_rhi = ENGINE.RDR;



	bool bCameraUsed = false;
	for (auto entity : entities_)
	{
		using namespace Components;

		CameraComponent& cameraComponent = p_em->GetComponent<CameraComponent>(entity);
		if (!cameraComponent.bEnabled)
			continue;

		LowRenderer::Cameras::Camera* p_camera = &cameraComponent.camera_;

		if (!p_camera->bUsed)
			continue;

		bCameraUsed = true;
		TransformComponent* p_transform = &p_em->GetComponent<TransformComponent>(entity);

		SetSkybox(p_camera, p_rhi);

		for (auto shader : p_shaders_)
		{
			shader->Use();
			p_rhi->SetCamera(p_camera, p_transform->position);
		}
	}
	if (!bCameraUsed)
	{
		SetSkybox((LowRenderer::Cameras::Camera*)p_rhi->GetCamera(), p_rhi);//Editor Camera
		for (auto shader : p_shaders_)
		{
			shader->Use();
			p_rhi->SetDefaultCamera(); //Editor Camera
		}
	}
}
void ECS::Systems::CameraSystem::RenderEditorScene()
{
	EntityManager* p_em = ENGINE.ENT_MNGR;
	Core::Renderer::RHI* p_rhi = ENGINE.RDR;

		SetSkybox((LowRenderer::Cameras::Camera*)p_rhi->GetCamera(), p_rhi);//Editor Camera
		for (auto shader : p_shaders_)
		{
			shader->Use();
			p_rhi->SetDefaultCamera(); //Editor Camera
		}
}

void ECS::Systems::CameraSystem::SetSkybox(LowRenderer::Cameras::Camera* _p_camera, Core::Renderer::RHI* _p_rhi)
{
	_p_rhi->DepthMaskActive(false);
	p_skyboxShader_->Use();
	
	_p_rhi->SetFixedCamera(_p_camera);
	_p_rhi->RenderCubeMap();
	_p_rhi->DepthMaskActive(true);
}