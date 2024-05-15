#pragma once

#include "ECS/Base/BaseComponent.hpp"
#include "Maths/Maths.hpp"
#include "LowRenderer/Meshes/MeshRenderer.hpp"

namespace ECS::Systems
{
	class MeshRendererSystem;
}

namespace ECS::Components
{
	struct MeshRendererComponent : public ECS::BaseComponent
	{
		std::string name;
		LowRenderer::Meshes::MeshRenderer renderer;
		TONIC_ENGINE_API MeshRendererComponent();
		TONIC_ENGINE_API MeshRendererComponent(std::string _name);
		TONIC_ENGINE_API MeshRendererComponent(Resources::MeshPtr _p_mesh);
		TONIC_ENGINE_API ~MeshRendererComponent();

	protected:
		friend class ECS::Systems::MeshRendererSystem;
		Maths::Mat4 model_ = 1.f;
	};
}