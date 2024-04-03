#pragma once
#include "ILight.hpp"

namespace ECS::Systems
{
	class PointLightSystem;
}

namespace LowRenderer
{
	class PointLight :
		public ILight
	{
	public:
		TONIC_ENGINE_API PointLight() = default;
		TONIC_ENGINE_API ~PointLight() = default;
		TONIC_ENGINE_API PointLight(const PointLight&) = default;

		friend class ECS::Systems::PointLightSystem;
	};
}