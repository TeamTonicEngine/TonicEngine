#pragma once

#include "ECS/Base/BaseComponent.hpp"
#include "Maths/Maths.hpp"

class Mesh;

namespace Components
{
	struct MeshRenderer : public ECS::BaseComponent
	{
		std::string name;
		Mesh* p_renderer; //could use shared_ptr

		TONIC_ENGINE_API MeshRenderer();
		TONIC_ENGINE_API MeshRenderer(std::string _name);
		TONIC_ENGINE_API ~MeshRenderer();
	};
}