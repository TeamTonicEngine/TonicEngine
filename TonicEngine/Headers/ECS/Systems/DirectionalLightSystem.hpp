#pragma once

#include "ECS/Base/BaseSystem.hpp"

namespace ECS::Systems
{
	class DirectionalLightSystem : public ECS::BaseSystem
	{
	private:
		std::vector<Resources::ShaderPtr> p_shaders_;

	public:
		TONIC_ENGINE_API DirectionalLightSystem();
		TONIC_ENGINE_API ~DirectionalLightSystem() = default;
		const bool TONIC_ENGINE_API Init() override;
		void TONIC_ENGINE_API Render() override;
		void TONIC_ENGINE_API RenderEditorScene() override;
	};
}