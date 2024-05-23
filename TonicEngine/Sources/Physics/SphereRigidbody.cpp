#include "pch.hpp"

#include "JoltInterfaceFunctions.cpp"

#include "Physics/SphereRigidbody.hpp"

// /!\ This implementation only works with the Jolt Physics Engine
#include "Physics/JoltWrapper.hpp"

#include <Jolt/Jolt.h>

#include <Jolt/Physics/Body/BodyInterface.h>

#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

Physics::SphereRigidbody::SphereRigidbody
(
	double _sphereRadius,
	Maths::Vec3 _spherePosition,
	Maths::Quat _sphereRotation, /* = identity */
	Motions::MotionType _motionType, /* = dynamic */
	bool _bActivateAtCreation /* = true */
)
{
	using namespace JPH; using namespace JPHInterface;

	SphereShapeSettings shapeSettings((float)_sphereRadius);
	shapeSettings.SetEmbedded();
	ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();

	BodyCreationSettings bodySettings(shapeResult.Get(),
		MathsToJolt(_spherePosition),
		MathsToJolt(_sphereRotation),
		Motions::GetJoltMotionFromMotionType(_motionType),
		Layers::GetLayerFromMotionType(_motionType)
	);

	auto bodyCreated = GetJoltInterface()->CreateBody(bodySettings);
	auto* bodyID = &bodyCreated->GetID();

	GetJoltInterface()->AddBody(*bodyID, (JPH::EActivation)!_bActivateAtCreation);

	ID = (void*)bodyID;
}

Physics::SphereRigidbody::~SphereRigidbody() { RemoveFromPhysic(); DestroyBody(); }

double Physics::SphereRigidbody::GetRadius() const
{
	using namespace JPHInterface;
	auto shape = GetJoltInterface()->GetShape(IDToJoltID(ID));
	auto size = (double)((JPH::SphereShape*)(shape.GetPtr()))->GetRadius();
	return size;
}

void Physics::SphereRigidbody::SetBounds(double _newRadius)
{
	using namespace JPH; using namespace JPHInterface;

	SphereShapeSettings shapeSettings((float)_newRadius);
	shapeSettings.SetEmbedded();
	ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();

	GetJoltInterface()->
		SetShape(IDToJoltID(ID),
			shapeResult.Get(),
			true,
			(JPH::EActivation)!IsActive()
		);
}