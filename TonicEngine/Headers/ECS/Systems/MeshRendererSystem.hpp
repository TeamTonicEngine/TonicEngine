#pragma once

#include "ECS/Base/BaseSystem.hpp"

namespace Resources {
	class Shader;
	class Material;
}

namespace ECS::Systems
{
	struct MeshRendererSystem : public ECS::BaseSystem
	{
	private:
		Resources::Shader* p_shader_;
		Resources::Shader* p_outlineShader_;

		//TODO: Uncomment on Material Component creation
		Resources::Material* p_defaultMaterial;

	public:
		TONIC_ENGINE_API MeshRendererSystem();
		TONIC_ENGINE_API ~MeshRendererSystem() = default;
		void TONIC_ENGINE_API Render() override;
	};
}