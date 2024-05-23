#pragma once
#include "LowRenderer/Lights/PointLight.hpp"
#include "ECS/Base/BaseComponent.hpp"

namespace ECS::Systems
{
	class PointLightSystem;
}
namespace ECS::Components
{
	struct PointLightComponent : public ECS::BaseComponent
	{
	public:
		TONIC_ENGINE_API PointLightComponent() = default;
		TONIC_ENGINE_API ~PointLightComponent() = default;
		TONIC_ENGINE_API PointLightComponent(const PointLightComponent&) = default;
		TONIC_ENGINE_API PointLightComponent(LowRenderer::Lights::PointLight& _light) : light_(_light) {};

		friend class ECS::Systems::PointLightSystem;

		_declspec(property(get = GetLight, put = SetLight))
			LowRenderer::Lights::PointLight light;
		inline TONIC_ENGINE_API LowRenderer::Lights::PointLight GetLight() const { return light_; };
		inline TONIC_ENGINE_API void SetLight(LowRenderer::Lights::PointLight _light) { light_ = _light; };

	protected:
		LowRenderer::Lights::PointLight light_; // problem if not in public
	};
}

