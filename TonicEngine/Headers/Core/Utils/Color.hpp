#pragma once

#include <Core/Utils/Type.hpp>
#include <Core/Utils/f32_4.hpp>
#include <Core/Utils/s32_3.hpp>

struct TONIC_ENGINE_API Color
{
	union
	{
		struct
		{
			u8 r, g, b, a;
		};
		u8 rgba[4];
	};

	f32_4 ToFloat();
	s32_3 ToHSV();
	s32 ToHex();
	u32 ToHexAlpha();

	static Color CreateRGBA(s32 _r, s32 _g, s32 _b, s32 _a);
	static Color CreateRGBAFloat(f32 _r, f32 _g, f32 _b, f32 _a);
	static Color CreateHSV(s32 _h, s32 _s, s32 _v);
	static Color CreateHexRGB(s32 hex);
	static Color CreateHexRGB_A(u32 hex);
};