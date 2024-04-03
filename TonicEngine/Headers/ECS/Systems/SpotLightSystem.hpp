#pragma once

#include "ECS/Base/BaseSystem.hpp"

namespace Resources {
	class Shader;
}

namespace ECS::Systems
{
	struct SpotLightSystem : public ECS::BaseSystem
	{
	private:
		Resources::Shader* p_shader_;

	public:
		TONIC_ENGINE_API SpotLightSystem();
		TONIC_ENGINE_API ~SpotLightSystem() = default;
		void TONIC_ENGINE_API Init();
		void TONIC_ENGINE_API Update() override;
	};
}