#include "pch.hpp"

#include "Core/Utils/Color.hpp"

#include <algorithm>
#include <cmath>

void Clamp(s32& _value, s32 min, s32 max)
{
	if (_value < min)
	{
		_value = min;
		return;
	}
	if (_value > max)
	{
		_value = max;
		return;
	}
}
void Clamp(f32& _value, f32 min, f32 max)
{
	if (_value < min)
	{
		_value = min;
		return;
	}
	if (_value > max)
	{
		_value = max;
		return;
	}
}

f32_4 Color::ToFloat()
{
	return { r / 255.f, g / 255.f, b / 255.f, a / 255.f };
}

s32_3 Color::ToHSV()
{
	s32_3 hsv = { 0,0,0 };

	s32 maxVal = std::max({ r, g, b });
	if (maxVal == 0)
		return hsv;

	s32 minVal = std::min({ r, g, b });
	s32 delta = maxVal - minVal;

	hsv.z = lroundf((maxVal * 100.f) / 255);

	hsv.y = lroundf((delta * 100.f) / maxVal);

	if (maxVal == r)
	{
		hsv.x = lround(60 * std::fmod((g - b) / (f32)delta, 6.0));
	}
	else if (maxVal == g)
	{
		hsv.x = lroundf(60 * (2 + (b - r) / (f32)delta));
	}
	else if (maxVal == b)
	{
		hsv.x = lroundf(60 * (4 + (r - g) / (f32)delta));
	}

	if (hsv.x < 0)
	{
		hsv.x += 360;
	}

	return hsv;
}

s32 Color::ToHex()
{
	return (r << 16) | (g << 8) | b;
}

u32 Color::ToHexAlpha()
{
	return (r << 24) | (g << 16) | b << 8 | a;
}

Color Color::CreateRGBA(s32 _r, s32 _g, s32 _b, s32 _a)
{
	Clamp(_r, 0, 255);
	Clamp(_g, 0, 255);
	Clamp(_b, 0, 255);
	Clamp(_a, 0, 255);
	return { static_cast<u8>(_r), static_cast<u8>(_g), static_cast<u8>(_b), static_cast<u8>(_a) };
}

Color Color::CreateRGBAFloat(f32 _r, f32 _g, f32 _b, f32 _a)
{
	Clamp(_r, 0.f, 1.f);
	Clamp(_g, 0.f, 1.f);
	Clamp(_b, 0.f, 1.f);
	Clamp(_a, 0.f, 1.f);
	return { static_cast<u8>(_r * 255), static_cast<u8>(_g * 255), static_cast<u8>(_b * 255), static_cast<u8>(_a * 255) };
}

Color Color::CreateHSV(s32 _h, s32 _s, s32 _v)
{
	u8 r = 0;
	u8 g = 0;
	u8 b = 0;
	u8 a = 255;

	if (_s == 0) {
		r = g = b = static_cast<u8>(lround((_v * 255) * 0.01f));
		return{ r,g,b,a };
	}

	f32 f, p, q, t, __h;
	s32 i;

	__h = _h / 60.f; // sector 0 to 5
	i = static_cast<s32>(floor(__h));
	f = __h - i; // fractional part of h
	p = (_v * 0.01f) * (1 - (_s * 0.01f));
	q = (_v * 0.01f) * (1 - (_s * 0.01f) * f);
	t = (_v * 0.01f) * (1 - (_s * 0.01f) * (1 - f));

	switch (i) {
	case 0:
		r = static_cast<u8>(lround((_v * 255) * 0.01f));
		g = static_cast<u8>(lround(t * 255));
		b = static_cast<u8>(lround(p * 255));
		break;
	case 1:
		r = static_cast<u8>(lround(q * 255));
		g = static_cast<u8>(lround((_v * 255) * 0.01f));
		b = static_cast<u8>(lround(p * 255));
		break;
	case 2:
		r = static_cast<u8>(lround(p * 255));
		g = static_cast<u8>(lround((_v * 255) * 0.01f));
		b = static_cast<u8>(lround(t * 255));
		break;
	case 3:
		r = static_cast<u8>(lround(p * 255));
		g = static_cast<u8>(lround(q * 255));
		b = static_cast<u8>(lround((_v * 255) * 0.01f));
		break;
	case 4:
		r = static_cast<u8>(lround(t * 255));
		g = static_cast<u8>(lround(p * 255));
		b = static_cast<u8>(lround((_v * 255) * 0.01f));
		break;
	default:        // case 5:
		r = static_cast<u8>(lround((_v * 255) * 0.01f));
		g = static_cast<u8>(lround(p * 255));
		b = static_cast<u8>(lround(q * 255));
		break;
	}
	return{ r,g,b,a };
}

Color Color::CreateHexRGB(s32 _hex)
{
	s32 r = (_hex >> 16) & 0xFF;
	s32 g = (_hex >> 8) & 0xFF;
	s32 b = _hex & 0xFF;
	return { static_cast<u8>(r),static_cast<u8>(g),static_cast<u8>(b) ,255 };
}
Color Color::CreateHexRGB_A(u32 _hex)
{
	s32 r = (_hex >> 24) & 0xFF;
	s32 g = (_hex >> 16) & 0xFF;
	s32 b = (_hex >> 8) & 0xFF;
	s32 a = _hex & 0xFF;
	return { static_cast<u8>(r),static_cast<u8>(g),static_cast<u8>(b) ,static_cast<u8>(a) };
}

//std::ostream& operator<<(std::ostream& os, const Color& color)
//{
//	os << "Color(RGBA): ["
//		<< static_cast<unsigned int>(color.r) << ", "
//		<< static_cast<unsigned int>(color.g) << ", "
//		<< static_cast<unsigned int>(color.b) << ", "
//		<< static_cast<unsigned int>(color.a) << "]";
//	return os;
//}
