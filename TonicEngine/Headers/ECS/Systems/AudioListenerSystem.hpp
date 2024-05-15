#pragma once

#include "ECS/Base/BaseSystem.hpp"

namespace ECS::Systems
{
	class AudioListenerSystem : public ECS::BaseSystem
	{
	public:
		TONIC_ENGINE_API AudioListenerSystem();
		TONIC_ENGINE_API ~AudioListenerSystem() = default;
		void TONIC_ENGINE_API Update() override;
	};
}