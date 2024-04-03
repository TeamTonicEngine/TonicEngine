#pragma once
#include "pch.hpp"
#include "Maths/MathsStream.hpp"

namespace Maths
{
	std::ostream& operator<<(std::ostream& _os, const Vec2& _vec)
	{
		return _os << '(' << _vec.x << ", " << _vec.y << ')';
	}
	std::ostream& operator<<(std::ostream& _os, const Vec3& _vec)
	{
		return _os << '(' << _vec.x << ", " << _vec.y << ", " << _vec.z << ')';
	}
	std::ostream& operator<<(std::ostream& _os, const Vec4& _vec)
	{
		return _os << '(' << _vec.x << ", " << _vec.y << ", " << _vec.z << ", " << _vec.w << ')';
	}
	std::ostream& operator<<(std::ostream& _os, const Quat& _quat)
	{
		return _os << '(' << _quat.x << ", " << _quat.y << ", " << _quat.z << ", " << _quat.w << ')';
	}
}