#pragma once
#include "ILight.hpp"

namespace ECS::Systems { class PointLightSystem; }
namespace Core::Renderer { class OpenGLWrapper; }

namespace LowRenderer::Lights
{
	class PointLight : public ILight
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		// Attenuation
		float constant_ = 1.f;
		float linear_ = 0.014f;
		float quadratic_ = 0.0007f;

	public:
		__declspec(property(get = GetConstant, put = SetConstant))
			float constant;

		_declspec(property(get = GetLinear, put = SetLinear))
			float linear;

		_declspec(property(get = GetQuadratic, put = SetQuadratic))
			float quadratic;

		friend class ECS::Systems::PointLightSystem;
		friend class Core::Renderer::OpenGLWrapper;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/

		TONIC_ENGINE_API PointLight() = default;
		TONIC_ENGINE_API PointLight(const PointLight&) = default;
		TONIC_ENGINE_API ~PointLight() = default;

		inline TONIC_ENGINE_API float GetConstant() const { return constant_; };
		inline TONIC_ENGINE_API void SetConstant(float _constant) { constant_ = _constant; };

		inline TONIC_ENGINE_API float GetLinear() const { return linear_; };
		inline TONIC_ENGINE_API void SetLinear(float _linear) { linear_ = _linear; };

		inline TONIC_ENGINE_API float GetQuadratic() const { return quadratic_; };
		inline TONIC_ENGINE_API void SetQuadratic(float _quadratic) { quadratic_ = _quadratic; };
	};
}