#pragma once

#include "ECS/Base/BaseSystem.hpp"

#include "Resources/Material.hpp"

namespace ECS::Systems
{
	class MeshRendererSystem : public ECS::BaseSystem
	{
	private:
		Resources::ShaderPtr p_outlineShader_;

		Resources::MaterialPtr p_defaultMaterial_;

		std::map<float/*distance2cam*/,EntityID> transparentEntities_;

	public:
		TONIC_ENGINE_API MeshRendererSystem();
		TONIC_ENGINE_API ~MeshRendererSystem();
		const bool TONIC_ENGINE_API Init() override;
		void TONIC_ENGINE_API Update() override;
		void TONIC_ENGINE_API Render() override;
		void TONIC_ENGINE_API RenderEditorScene() override;
	};
}