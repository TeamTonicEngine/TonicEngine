#pragma once
#include <pch.hpp>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyInterface.h>

#include <Maths/Maths.hpp>

#include <Core/Engine.hpp>

namespace Physics::JPHInterface
{
	static JPH::Vec3 MathsToJolt(Maths::Vec3 _v) { return JPH::Vec3(_v.x, _v.y, _v.z); }
	static Maths::Vec3 JoltToMaths(JPH::Vec3 _v) { return Maths::Vec3(_v.GetX(), _v.GetY(), _v.GetZ()); }

	static JPH::Quat MathsToJolt(Maths::Quat _q) { return JPH::Quat(_q.x, _q.y, _q.z, _q.w); }
	static Maths::Quat JoltToMaths(JPH::Quat _q) { return Maths::Quat(_q.GetW(), _q.GetX(), _q.GetY(), _q.GetZ()); }

	static JPH::BodyID IDToJoltID(void* _ptr) { return *(JPH::BodyID*)(_ptr); }
	static JPH::BodyInterface* GetJoltInterface() { return (JPH::BodyInterface*)ENGINE.PHY_ENG->GetBodyInterface(); }
}