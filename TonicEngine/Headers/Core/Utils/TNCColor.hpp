#pragma once

#include <Core/Utils/Type.hpp>
#include <Core/Utils/f32_4.hpp>
#include <Core/Utils/s32_3.hpp>

struct TONIC_ENGINE_API TNCColor
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
	Maths::Vec3 ToVector3() const;
	Maths::Vec4 ToVector4() const;

	static TNCColor CreateRGBA(s32 _r, s32 _g, s32 _b, s32 _a);
	static TNCColor CreateRGBAFloat(f32 _r, f32 _g, f32 _b, f32 _a);
	static TNCColor CreateHSV(s32 _h, s32 _s, s32 _v);
	static TNCColor CreateHexRGB(s32 hex);
	static TNCColor CreateHexRGB_A(u32 hex);

	TNCColor operator*(float _mult) const;
};