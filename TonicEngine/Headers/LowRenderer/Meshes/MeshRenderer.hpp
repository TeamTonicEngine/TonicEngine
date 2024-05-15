#pragma once

#include <Resources/Material.hpp>

namespace LowRenderer::Meshes
{
	struct MeshRenderer
	{
		Resources::MeshPtr p_mesh;
		std::vector <Resources::MaterialPtr> p_materials;

		TONIC_ENGINE_API MeshRenderer() {};
		TONIC_ENGINE_API MeshRenderer(std::string _name) { p_mesh = ENGINE.RES_MNGR->Get<Resources::Mesh>(_name); if (!p_mesh) ENGINE.RES_MNGR->Create<Resources::Mesh>(_name); };
		TONIC_ENGINE_API ~MeshRenderer() {};
	};
}