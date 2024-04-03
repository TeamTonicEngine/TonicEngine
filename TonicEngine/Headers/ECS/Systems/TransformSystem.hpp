#pragma once

#include "ECS/Base/BaseSystem.hpp"

namespace ECS::Systems
{
	struct TransformSystem : public ECS::BaseSystem
	{
	public:
		TONIC_ENGINE_API TransformSystem();
		TONIC_ENGINE_API ~TransformSystem() = default;
		void TONIC_ENGINE_API Update() override;
	};
}