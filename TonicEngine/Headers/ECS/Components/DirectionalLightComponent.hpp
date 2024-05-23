#pragma once

#include "ECS/Base/BaseComponent.hpp"

#include "LowRenderer/Lights/DirectionalLight.hpp"

namespace ECS::Systems { class DirectionalLightSystem; }

namespace ECS::Components
{
	struct DirectionalLightComponent :
		public ECS::BaseComponent
	{
	public:

		TONIC_ENGINE_API DirectionalLightComponent() = default;
		TONIC_ENGINE_API ~DirectionalLightComponent() = default;
		TONIC_ENGINE_API DirectionalLightComponent(const DirectionalLightComponent&) = default;
		TONIC_ENGINE_API DirectionalLightComponent(LowRenderer::Lights::DirectionalLight& _light) : light_(_light) { };

		friend class ECS::Systems::DirectionalLightSystem;

		__declspec(property(get = GetLight, put = SetLight))
			LowRenderer::Lights::DirectionalLight light;

		inline LowRenderer::Lights::DirectionalLight TONIC_ENGINE_API GetLight() const { return light_; };
		inline void TONIC_ENGINE_API SetLight(LowRenderer::Lights::DirectionalLight _light) { light_ = _light; };

	protected:
		LowRenderer::Lights::DirectionalLight light_;
	};
}