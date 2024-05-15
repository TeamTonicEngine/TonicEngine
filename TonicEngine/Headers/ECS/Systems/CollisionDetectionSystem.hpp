#pragma once

#include "ECS/Base/BaseSystem.hpp"

namespace ECS::Systems
{
	class CollisionDetectionSystem : public ECS::BaseSystem
	{
	public:
		TONIC_ENGINE_API CollisionDetectionSystem();
		TONIC_ENGINE_API ~CollisionDetectionSystem() = default;
		void TONIC_ENGINE_API Update() override;
	};
}