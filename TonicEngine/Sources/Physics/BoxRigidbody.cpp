#include "pch.hpp"

#include "JoltInterfaceFunctions.cpp"

#include "Physics/BoxRigidbody.hpp"

// /!\ This implementation only works with the Jolt Physics Engine
#include "Physics/JoltWrapper.hpp"

#include <Jolt/Jolt.h>

#include <Jolt/Physics/Body/BodyInterface.h>

#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

Physics::BoxRigidbody::BoxRigidbody
(
	Maths::Vec3 _boxExtents,
	Maths::Vec3 _boxPosition,
	Maths::Quat _boxRotation, /* = identity */
	Motions::MotionType _motionType, /* = dynamic */
	bool _bActivateAtCreation /* = true */
)
{
	using namespace JPH; using namespace JPHInterface;

	BoxShapeSettings shapeSettings(MathsToJolt(_boxExtents));
	shapeSettings.SetEmbedded();
	ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();

	BodyCreationSettings bodySettings(shapeResult.Get(),
		MathsToJolt(_boxPosition),
		MathsToJolt(_boxRotation),
		Motions::GetJoltMotionFromMotionType(_motionType),
		Layers::GetLayerFromMotionType(_motionType)
	);

	auto bodyCreated = GetJoltInterface()->CreateBody(bodySettings);
	auto* bodyID = &bodyCreated->GetID();

	GetJoltInterface()->AddBody(*bodyID, (JPH::EActivation)!_bActivateAtCreation);

	ID = (void*)bodyID;
}

Physics::BoxRigidbody::~BoxRigidbody() { RemoveFromPhysic(); DestroyBody(); }

Maths::Vec3 Physics::BoxRigidbody::GetBounds() const
{
	using namespace JPHInterface;
	auto shape = GetJoltInterface()->GetShape(IDToJoltID(ID));
	auto size = ((JPH::BoxShape*)(shape.GetPtr()))->GetLocalBounds().GetSize();
	return JoltToMaths(size);
}

void Physics::BoxRigidbody::SetBounds(Maths::Vec3 _newBounds)
{
	using namespace JPH; using namespace JPHInterface;

	BoxShapeSettings shapeSettings(MathsToJolt(_newBounds / 2.f));
	shapeSettings.SetEmbedded();
	ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();

	GetJoltInterface()->
		SetShape(IDToJoltID(ID),
			shapeResult.Get(),
			true,
			(JPH::EActivation)!IsActive()
		);
}