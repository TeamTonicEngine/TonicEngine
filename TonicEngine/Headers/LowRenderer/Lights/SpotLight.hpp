#pragma once
#include "ILight.hpp"

namespace ECS::Systems { class SpotLightSystem; }
namespace Core::Renderer { class OpenGLWrapper; }

namespace LowRenderer::Lights
{
	class SpotLight : public ILight
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		// CutOff
		float inCutOff_ = 15.0f;
		float outCutOff_ = 40.5f;

		// Attenuation
		float constant_ = 1.f;
		float linear_ = 0.014f;
		float quadratic_ = 0.0007f;

	public:
		__declspec(property(get = GetInCutOff, put = SetInCutOff))
			float inCutOff;

		__declspec(property(get = GetOutCutOff, put = SetOutCutOff))
			float outCutOff;

		__declspec(property(get = GetConstant, put = SetConstant))
			float constant;

		_declspec(property(get = GetLinear, put = SetLinear))
			float linear;

		_declspec(property(get = GetQuadratic, put = SetQuadratic))
			float quadratic;

		friend class ECS::Systems::SpotLightSystem;
		friend class Core::Renderer::OpenGLWrapper;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/

		TONIC_ENGINE_API SpotLight() = default;
		TONIC_ENGINE_API SpotLight(const SpotLight&) = default;
		TONIC_ENGINE_API ~SpotLight() = default;

		inline TONIC_ENGINE_API float GetInCutOff() const { return inCutOff_; };
		inline TONIC_ENGINE_API void SetInCutOff(float _inCutOff) { inCutOff_ = _inCutOff; };

		inline TONIC_ENGINE_API float GetOutCutOff() const { return outCutOff_; };
		inline TONIC_ENGINE_API void SetOutCutOff(float _outCutOff) { outCutOff_ = _outCutOff; };

		inline TONIC_ENGINE_API float GetConstant() const { return constant_; };
		inline TONIC_ENGINE_API void SetConstant(float _constant) { constant_ = _constant; };

		inline TONIC_ENGINE_API float GetLinear() const { return linear_; };
		inline TONIC_ENGINE_API void SetLinear(float _linear) { linear_ = _linear; };

		inline TONIC_ENGINE_API float GetQuadratic() const { return quadratic_; };
		inline TONIC_ENGINE_API void SetQuadratic(float _quadratic) { quadratic_ = _quadratic; };
	};
}