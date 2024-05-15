#pragma once

#include "ECS/Base/BaseSystem.hpp"

namespace Resources {
	class Shader;
}

namespace ECS::Systems
{
	class SpotLightSystem : public ECS::BaseSystem
	{
	private:
		std::vector<Resources::ShaderPtr> p_shaders_;

	public:
		TONIC_ENGINE_API SpotLightSystem();
		TONIC_ENGINE_API ~SpotLightSystem() = default;
		const bool TONIC_ENGINE_API Init() override;
		void TONIC_ENGINE_API Render() override;
		void TONIC_ENGINE_API RenderEditorScene() override;
	};
}