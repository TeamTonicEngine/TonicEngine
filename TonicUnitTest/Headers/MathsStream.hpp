#pragma once
#include <iostream>
#include "Maths.hpp"

namespace Maths
{
	std::ostream& operator<<(std::ostream& _os, const Vec2& _vec);
	std::ostream& operator<<(std::ostream& _os, const Vec3& _vec);
	std::ostream& operator<<(std::ostream& _os, const Vec4& _vec);
}