#pragma once
#include "ILight.hpp"

namespace ECS::Systems
{
	struct SpotLightSystem;
}

namespace LowRenderer
{
	struct SpotLight :
		public ILight
	{
	public:
		TONIC_ENGINE_API SpotLight() = default;
		TONIC_ENGINE_API ~SpotLight() = default;
		TONIC_ENGINE_API SpotLight(const SpotLight&) = default;

		friend ECS::Systems::SpotLightSystem;

	protected:
		float inCutOff_ = 55.0f;
		float outCutOff_ = 140.5f;
	};
}