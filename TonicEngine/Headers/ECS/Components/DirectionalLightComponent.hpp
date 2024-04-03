#pragma once
#include "LowRenderer/DirectionalLight.hpp"
#include "ECS/Base/BaseComponent.hpp"

namespace ECS::Systems
{
	struct DirectionalLightSystem;
}

namespace Components
{
struct DirectionalLightComponent :
    public ECS::BaseComponent
{
public:
	TONIC_ENGINE_API DirectionalLightComponent() = default;
	TONIC_ENGINE_API ~DirectionalLightComponent() = default;
	TONIC_ENGINE_API DirectionalLightComponent(const DirectionalLightComponent&) = default;
	TONIC_ENGINE_API DirectionalLightComponent(LowRenderer::DirectionalLight& _light) : p_light_(&_light) {};

	friend struct ECS::Systems::DirectionalLightSystem;

protected:
	LowRenderer::DirectionalLight* p_light_;
};
}

