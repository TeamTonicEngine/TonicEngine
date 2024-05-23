#pragma once
#include "DLL_API.hpp"

#include "Core/Utils/TNCColor.hpp"

namespace ECS::Systems
{
	class PointLightSystem;
	class DirectionalLightSystem;
	class SpotLightSystem;
}

namespace LowRenderer::Lights
{
	class ILight
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		TNCColor color_ = TNCColor::CreateRGBAFloat(1.f, 1.f, 1.f, 1.f);
		bool bShadow_;
		f32 bias_ = 0.05f;
		f32 strength_ = 1.0f;

	public:
		__declspec(property(get = GetColor, put = SetColor))
			TNCColor color;

		__declspec(property(get = IsShadowed, put = SetShadowed))
			bool bShadow;

		__declspec(property(get = GetBias, put = SetBias))
			float bias;

		__declspec(property(get = GetStrength, put = SetStrength))
			float strength;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/

		inline TONIC_ENGINE_API TNCColor GetColor() const { return color_; }
		inline TONIC_ENGINE_API void SetColor(const TNCColor& _color) { color_ = _color; }

		inline TONIC_ENGINE_API bool IsShadowed() const { return bShadow_; }
		inline TONIC_ENGINE_API void SetShadowed(bool _bShadow) { bShadow_ = _bShadow; }

		inline TONIC_ENGINE_API float GetBias() const { return bias_; }
		inline TONIC_ENGINE_API void SetBias(float _bias) { bias_ = _bias; }

		inline TONIC_ENGINE_API float GetStrength() const { return strength_; };
		inline TONIC_ENGINE_API void SetStrength(float _strength) { strength_ = _strength; };
	};
}