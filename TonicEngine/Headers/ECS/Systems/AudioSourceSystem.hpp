#pragma once

#include "ECS/Base/BaseSystem.hpp"

namespace ECS::Systems
{
	class AudioSourceSystem : public ECS::BaseSystem
	{
	public:
		TONIC_ENGINE_API AudioSourceSystem();
		TONIC_ENGINE_API ~AudioSourceSystem() = default;
		void TONIC_ENGINE_API Update() override;
	};
}