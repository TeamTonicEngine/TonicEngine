#pragma once
#include <Resources/Material.hpp>

namespace LowRenderer::Meshes
{
	struct MeshRenderer
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
		Resources::MeshPtr p_mesh;
		std::vector <Resources::MaterialPtr> p_materials;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/

		TONIC_ENGINE_API MeshRenderer() = default;
		TONIC_ENGINE_API MeshRenderer(std::string _name)
		{
			p_mesh = ENGINE.RES_MNGR->Get<Resources::Mesh>(_name);
			if (!p_mesh)
				ENGINE.RES_MNGR->Create<Resources::Mesh>(_name);
		};
		TONIC_ENGINE_API ~MeshRenderer() = default;
	};
}