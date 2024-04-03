#pragma once

#include "ECS/Base/BaseSystem.hpp"
#include "ECS/Base/EntityManager.hpp"

#include "ECS/Components/Camera.hpp"
#include "ECS/Components/Transform.hpp"

namespace Resources
{
	class Shader;
}

namespace ECS::Systems
{
	struct CameraSystem : public ECS::BaseSystem
	{

	public:
		float alpha = 0.f;
		TONIC_ENGINE_API CameraSystem();
		TONIC_ENGINE_API ~CameraSystem() = default;
		//void TONIC_ENGINE_API Render() override;
		void TONIC_ENGINE_API Update() override;


	private:
		std::vector<Resources::Shader*> p_shaders_;
		Resources::Shader* p_skyboxShader_;
		//TODO: Uncomment on Material Component creation

		//Resources::Material defaultMaterial_;
	};
}