#pragma once

#include "ECS/Base/BaseSystem.hpp"
#include "ECS/Base/EntityManager.hpp"

#include "ECS/Components/CameraComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"

namespace Resources
{
	class Shader;
}

namespace ECS::Systems
{
	class CameraSystem : public ECS::BaseSystem
	{
	public:
		float alpha = 0.f; // TODO Delete or Use ...
		TONIC_ENGINE_API CameraSystem();
		TONIC_ENGINE_API ~CameraSystem() = default;
		const bool TONIC_ENGINE_API Init() override;

		void Destroy();

		void TONIC_ENGINE_API Render() override;
		void TONIC_ENGINE_API RenderEditorScene() override;
		void TONIC_ENGINE_API Update() override;

	private:
		std::vector<Resources::ShaderPtr> p_shaders_;
		Resources::ShaderPtr p_skyboxShader_;

		void SetSkybox(LowRenderer::Cameras::Camera* _camera, Core::Renderer::RHI* _p_rhi);
	};
}