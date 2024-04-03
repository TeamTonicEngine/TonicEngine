#pragma once

#include "ECS/Base/BaseSystem.hpp"

namespace Resources {
	class Shader;
}

namespace ECS::Systems
{
	struct ModelRendererSystem : public ECS::BaseSystem
	{
	private:
		Resources::Shader* p_shader_;

	public:
		TONIC_ENGINE_API ModelRendererSystem();
		TONIC_ENGINE_API ~ModelRendererSystem() = default;
		void TONIC_ENGINE_API Render() override;
	};
}