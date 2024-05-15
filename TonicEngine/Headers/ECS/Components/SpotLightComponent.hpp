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
		TONIC_ENGINE_API SpotLightComponent(LowRenderer::Lights::SpotLight& _light) : light_(_light) {};

		friend class ECS::Systems::SpotLightSystem;

		_declspec(property(get = GetLight, put = SetLight))
			LowRenderer::Lights::SpotLight light;
		inline TONIC_ENGINE_API LowRenderer::Lights::SpotLight GetLight() const { return light_; };
		inline TONIC_ENGINE_API void SetLight(LowRenderer::Lights::SpotLight _light) { light_ = _light; };

	protected:
		LowRenderer::Lights::SpotLight light_;
};
}

