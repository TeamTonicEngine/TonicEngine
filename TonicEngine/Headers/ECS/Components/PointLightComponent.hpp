#pragma once
#include "LowRenderer/PointLight.hpp"
#include "ECS/Base/BaseComponent.hpp"

namespace ECS::Systems
{
	struct PointLightSystem;
}
namespace Components
{
struct PointLightComponent : public ECS::BaseComponent
{
public:
	TONIC_ENGINE_API PointLightComponent() = default;
	TONIC_ENGINE_API ~PointLightComponent() = default;
	TONIC_ENGINE_API PointLightComponent(const PointLightComponent&) = default;
	TONIC_ENGINE_API PointLightComponent(LowRenderer::PointLight& _light) : p_light_(&_light) {};

	friend struct ECS::Systems::PointLightSystem;
protected:
	LowRenderer::PointLight* p_light_;
};
}

