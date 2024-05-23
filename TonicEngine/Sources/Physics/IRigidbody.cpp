#include "pch.hpp"

#include "JoltInterfaceFunctions.cpp"

#include "Physics/IRigidbody.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyInterface.h>

#include "Physics/JoltWrapper.hpp"

void Physics::IRigidbody::SetEnable(bool _bEnable)
{
	using namespace JPHInterface;
	if (_bEnable)
		GetJoltInterface()->ActivateBody(IDToJoltID(ID));
	else
		GetJoltInterface()->DeactivateBody(IDToJoltID(ID));
}

bool Physics::IRigidbody::GetEnable() const
{
	using namespace JPHInterface;
	return GetJoltInterface()->IsActive(IDToJoltID(ID));
}

void Physics::IRigidbody::SetCenter(Maths::Vec3 _newCenter)
{
	using namespace JPHInterface;
	GetJoltInterface()->SetPosition(IDToJoltID(ID),
		MathsToJolt(_newCenter),
		(JPH::EActivation)!IsActive());
}

Maths::Vec3 Physics::IRigidbody::GetCenter() const
{
	using namespace JPHInterface;
	return JoltToMaths(GetJoltInterface()->GetPosition(IDToJoltID(ID)));
}

void Physics::IRigidbody::SetRotation(Maths::Quat _newRotation)
{
	using namespace JPHInterface;
	GetJoltInterface()->SetRotation(IDToJoltID(ID),
		MathsToJolt(_newRotation),
		(JPH::EActivation)!IsActive());
}

Maths::Quat Physics::IRigidbody::GetRotation() const
{
	using namespace JPHInterface;
	return JoltToMaths(GetJoltInterface()->GetRotation(IDToJoltID(ID)));
}

void Physics::IRigidbody::SetLinearVelocity(Maths::Vec3 _newVelocity)
{
	using namespace JPHInterface;
	GetJoltInterface()->SetLinearVelocity(IDToJoltID(ID),
		MathsToJolt(_newVelocity));
}

Maths::Vec3 Physics::IRigidbody::GetLinearVelocity() const
{
	using namespace JPHInterface;
	return JoltToMaths(GetJoltInterface()->GetLinearVelocity(IDToJoltID(ID)));
}

void Physics::IRigidbody::SetAngularVelocity(Maths::Vec3 _newVelocity)
{
	using namespace JPHInterface;
	GetJoltInterface()->SetAngularVelocity(IDToJoltID(ID),
		MathsToJolt(_newVelocity));
}

Maths::Vec3 Physics::IRigidbody::GetAngularVelocity() const
{
	using namespace JPHInterface;
	return JoltToMaths(GetJoltInterface()->GetAngularVelocity(IDToJoltID(ID)));
}

void Physics::IRigidbody::SetAngularAndLinearVelocity(Maths::Vec3 _newAngular, Maths::Vec3 _newLinear)
{
	using namespace JPHInterface;
	GetJoltInterface()->SetLinearAndAngularVelocity(IDToJoltID(ID),
		MathsToJolt(_newLinear), MathsToJolt(_newAngular));
}

void Physics::IRigidbody::SetMotionType(Motions::MotionType _newMotionType)
{
	using namespace JPHInterface;
	GetJoltInterface()->SetMotionType(IDToJoltID(ID),
		Motions::GetJoltMotionFromMotionType(_newMotionType),
		(JPH::EActivation)!IsActive());
}

Physics::Motions::MotionType Physics::IRigidbody::GetMotionType() const
{
	using namespace JPHInterface;
	return Motions::GetMotionTypeFromJoltMotion(
		GetJoltInterface()->GetMotionType(IDToJoltID(ID)));
}

bool Physics::IRigidbody::IsActive()
{
	using namespace JPHInterface;
	return GetJoltInterface()->IsActive(IDToJoltID(ID));
}

void Physics::IRigidbody::RemoveFromPhysic()
{
	using namespace JPHInterface;
	GetJoltInterface()->RemoveBody(IDToJoltID(ID));
}

void Physics::IRigidbody::DestroyBody()
{
	using namespace JPHInterface;
	GetJoltInterface()->DestroyBody(IDToJoltID(ID));
}