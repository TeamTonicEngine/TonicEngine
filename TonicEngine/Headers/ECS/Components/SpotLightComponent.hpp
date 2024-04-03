#pragma once
#include "LowRenderer/SpotLight.hpp"
#include "ECS/Base/BaseComponent.hpp"

namespace ECS::Systems
{
	struct SpotLightSystem;
}

namespace Components
{
	struct SpotLightComponent : public ECS::BaseComponent
	{
	public:
		TONIC_ENGINE_API SpotLightComponent() = default;
		TONIC_ENGINE_API ~SpotLightComponent() = default;
		TONIC_ENGINE_API SpotLightComponent(const SpotLightComponent&) = default;
		TONIC_ENGINE_API SpotLightComponent(LowRenderer::SpotLight& _light) : p_light_(&_light) {};

	friend struct ECS::Systems::SpotLightSystem;
protected:
	LowRenderer::SpotLight* p_light_;
};
}

