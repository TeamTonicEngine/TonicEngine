#include "pch.hpp"

#include "JoltInterfaceFunctions.cpp"

#include "Physics/CapsuleRigidbody.hpp"

// /!\ This implementation only works with the Jolt Physics Engine
#include "Physics/JoltWrapper.hpp"

#include <Jolt/Jolt.h>

#include <Jolt/Physics/Body/BodyInterface.h>

#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

Physics::CapsuleRigidbody::CapsuleRigidbody
(
	double _capsuleRadius,
	double _capsuleHeight,
	Maths::Vec3 _capsulePosition,
	Maths::Quat _capsuleRotation, /* = identity */
	Motions::MotionType _motionType, /* = dynamic */
	bool _bActivateAtCreation /* = true */
)
{
	using namespace JPH; using namespace JPHInterface;

	CapsuleShapeSettings shapeSettings((float)_capsuleHeight, (float)_capsuleRadius);
	shapeSettings.SetEmbedded();
	ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();

	BodyCreationSettings bodySettings(shapeResult.Get(),
		MathsToJolt(_capsulePosition),
		MathsToJolt(_capsuleRotation),
		Motions::GetJoltMotionFromMotionType(_motionType),
		Layers::GetLayerFromMotionType(_motionType)
	);

	auto bodyCreated = GetJoltInterface()->CreateBody(bodySettings);
	auto* bodyID = &bodyCreated->GetID();

	GetJoltInterface()->AddBody(*bodyID, (JPH::EActivation)!_bActivateAtCreation);

	ID = (void*)bodyID;
}

Physics::CapsuleRigidbody::~CapsuleRigidbody() { RemoveFromPhysic(); DestroyBody(); }

double Physics::CapsuleRigidbody::GetHeight() const
{
	using namespace JPHInterface;
	auto shape = GetJoltInterface()->GetShape(IDToJoltID(ID));
	auto size = (double)((JPH::CapsuleShape*)(shape.GetPtr()))->GetHalfHeightOfCylinder() * 2;
	return size;
}

double Physics::CapsuleRigidbody::GetRadius() const
{
	using namespace JPHInterface;
	auto shape = GetJoltInterface()->GetShape(IDToJoltID(ID));
	auto size = (double)((JPH::CapsuleShape*)(shape.GetPtr()))->GetRadius();
	return size;
}

void Physics::CapsuleRigidbody::SetBounds(double _newRadius, double _newHeight)
{
	using namespace JPH; using namespace JPHInterface;

	CapsuleShapeSettings shapeSettings((float)_newHeight, (float)_newRadius);
	shapeSettings.SetEmbedded();
	ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();

	GetJoltInterface()->
		SetShape(IDToJoltID(ID),
			shapeResult.Get(),
			true,
			(JPH::EActivation)!IsActive()
		);
}