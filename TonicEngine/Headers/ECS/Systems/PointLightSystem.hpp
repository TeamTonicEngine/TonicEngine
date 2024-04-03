#pragma once

#include "ECS/Base/BaseSystem.hpp"

namespace Resources {
	class Shader;
}

namespace ECS::Systems
{
	struct PointLightSystem : public ECS::BaseSystem
	{
	private:
		Resources::Shader* p_shader_;

	public:
		TONIC_ENGINE_API PointLightSystem();
		TONIC_ENGINE_API ~PointLightSystem() = default;
		void TONIC_ENGINE_API Init();
		void TONIC_ENGINE_API Update() override;


		void TONIC_ENGINE_API Render() override;
	};
}