#pragma once
#include "LowRenderer/Lights/SpotLight.hpp"
#include "ECS/Base/BaseComponent.hpp"

namespace ECS::Systems
{
	class SpotLightSystem;
}

namespace ECS::Components
{
	struct SpotLightComponent : public ECS::BaseComponent
	{
	public:
		TONIC_ENGINE_API SpotLightComponent()  = default;
		TONIC_ENGINE_API ~SpotLightComponent() = default;
		TONIC_ENGINE_API SpotLightComponent(const SpotLightComponent&) = default;
		TONIC_ENGINE_API SpotLightComponent(LowRenderer::Lights::SpotLight& _light) : light(_light) {};

		friend class ECS::Systems::SpotLightSystem;

		LowRenderer::Lights::SpotLight light;
};
}

