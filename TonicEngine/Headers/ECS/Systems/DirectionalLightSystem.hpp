#pragma once

#include "ECS/Base/BaseSystem.hpp"

namespace Resources {
	class Shader;
}

namespace ECS::Systems
{
	struct DirectionalLightSystem : public ECS::BaseSystem
	{
	private:
		Resources::Shader* p_shader_;

	public:
		TONIC_ENGINE_API DirectionalLightSystem();
		TONIC_ENGINE_API ~DirectionalLightSystem() = default;
		void TONIC_ENGINE_API Init();
		void TONIC_ENGINE_API Update() override;
	};
}