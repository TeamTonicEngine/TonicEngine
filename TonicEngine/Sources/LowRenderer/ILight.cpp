#include "pch.hpp"
#include "LowRenderer/ILight.hpp"

Color LowRenderer::ILight::GetColor() const
{
	return color_;
}

bool LowRenderer::ILight::IsShadowed() const
{
	return bShadow_;
}

float LowRenderer::ILight::GetBias() const
{
	return bias_;
}

float LowRenderer::ILight::GetStrength() const
{
	return strength_;
}
