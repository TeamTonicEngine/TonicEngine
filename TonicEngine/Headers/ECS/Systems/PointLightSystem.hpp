#pragma once

#include "ECS/Base/BaseSystem.hpp"

namespace Resources {
	class Shader;
}

namespace ECS::Systems
{
	class PointLightSystem: public ECS::BaseSystem
	{
	private:
		std::vector<Resources::ShaderPtr> p_shaders_;

	public:
		TONIC_ENGINE_API PointLightSystem();
		TONIC_ENGINE_API ~PointLightSystem() = default;
		const bool TONIC_ENGINE_API Init() override;

		void TONIC_ENGINE_API Render() override;
		void TONIC_ENGINE_API RenderEditorScene() override;
	};
}