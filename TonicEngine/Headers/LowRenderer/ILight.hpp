#pragma once
#include "DLL_API.hpp"
#include "Core/Utils/Color.hpp"

namespace ECS::Systems
{
	class PointLightSystem;
	class DirectionalLightSystem;
	class potLightSystem;
}
namespace LowRenderer
{
	class ILight
	{
		__declspec(property(get = GetColor))
			Color color;
		Color GetColor() const;
		__declspec(property(get = IsShadowed))
			bool bShadow;
		bool IsShadowed() const;
		__declspec(property(get = GetBias))
			float bias;
		float GetBias() const;
		__declspec(property(get = GetStrength))
			float strength;
		float GetStrength() const;

	protected:
		Color color_ = Color::CreateRGBAFloat(1.f, 1.f, 1.f, 1.f);
		bool bShadow_;
		float bias_ = 0.05f;
		float strength_ = 10.0f;
	};
}
