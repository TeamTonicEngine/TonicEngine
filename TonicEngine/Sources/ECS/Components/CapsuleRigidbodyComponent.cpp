#include "pch.hpp"

#include "Physics/CapsuleRigidbody.hpp"
#include "ECS/Components/CapsuleRigidbodyComponent.hpp"

template TONIC_ENGINE_API const ECS::ComponentTypeID ECS::CompType<ECS::Components::CapsuleRigidbodyComponent>() noexcept;

ECS::Components::CapsuleRigidbodyComponent::CapsuleRigidbodyComponent()
	: CapsuleRigidbodyComponent(1., 1., Maths::Vec3(0))
{}

ECS::Components::CapsuleRigidbodyComponent::CapsuleRigidbodyComponent(
	double _capsuleHeight,
	double _capsuleRadius,
	Maths::Vec3 _capsulePosition,
	Maths::Quat _capsuleRotation, /* = identity */
	Physics::Motions::MotionType _motionType, /* = dynamic */
	bool _bActivateMovementAtCreation /* = true */
) {
	capsuleHeight_ = _capsuleHeight;
	capsuleRadius_ = _capsuleRadius;
	centerOffset_ = _capsulePosition;
	rotationOffset_ = _capsuleRotation;
	motionType_ = _motionType;
	bMovementAtStart_ = _bActivateMovementAtCreation;
}

ECS::Components::CapsuleRigidbodyComponent::~CapsuleRigidbodyComponent() { DestroyRigidbody(); }

void ECS::Components::CapsuleRigidbodyComponent::Init
(
	Maths::Vec3 _size,
	Maths::Vec3 _position,
	Maths::Quat _rotation
) {
	p_rigidbody_ = std::make_shared<Physics::CapsuleRigidbody>(
		capsuleRadius_,
		capsuleHeight_,
		_position + centerOffset_,
		_rotation * rotationOffset_,
		motionType_,
		bMovementAtStart_);

	rotationParent = _rotation;
	center = _position;
}

double ECS::Components::CapsuleRigidbodyComponent::GetHeight() const
{
	return std::dynamic_pointer_cast<Physics::CapsuleRigidbody>(p_rigidbody_)->GetHeight();
}

void ECS::Components::CapsuleRigidbodyComponent::SetHeight(double _newHeight)
{
	capsuleHeight_ = std::max(_newHeight, .2);
	std::dynamic_pointer_cast<Physics::CapsuleRigidbody>(p_rigidbody_)->
		SetBounds(capsuleRadius_, capsuleHeight_);
}

double ECS::Components::CapsuleRigidbodyComponent::GetRadius() const
{
	return std::dynamic_pointer_cast<Physics::CapsuleRigidbody>(p_rigidbody_)->GetRadius();
}

void ECS::Components::CapsuleRigidbodyComponent::SetRadius(double _newRadius)
{
	capsuleRadius_ = std::max(_newRadius, .15);
	std::dynamic_pointer_cast<Physics::CapsuleRigidbody>(p_rigidbody_)->
		SetBounds(capsuleRadius_, capsuleHeight_);
}