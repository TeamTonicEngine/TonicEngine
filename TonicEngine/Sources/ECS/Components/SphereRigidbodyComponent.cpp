#include "pch.hpp"

#include "Physics/SphereRigidbody.hpp"
#include "ECS/Components/SphereRigidbodyComponent.hpp"

template TONIC_ENGINE_API const ECS::ComponentTypeID ECS::CompType<ECS::Components::SphereRigidbodyComponent>() noexcept;

ECS::Components::SphereRigidbodyComponent::SphereRigidbodyComponent()
	: SphereRigidbodyComponent(1., Maths::Vec3(0))
{}

ECS::Components::SphereRigidbodyComponent::SphereRigidbodyComponent
(
	double _sphereRadius,
	Maths::Vec3 _spherePosition,
	Maths::Quat _sphereRotation, /* = identity */
	Physics::Motions::MotionType _motionType, /* = dynamic */
	bool _bActivateMovementAtCreation /* = true */
)
{
	sphereRadius_ = _sphereRadius;
	centerOffset_ = _spherePosition;
	rotationOffset_ = _sphereRotation;
	motionType_ = _motionType;
	bMovementAtStart_ = _bActivateMovementAtCreation;
}

ECS::Components::SphereRigidbodyComponent::~SphereRigidbodyComponent() { DestroyRigidbody(); }

void ECS::Components::SphereRigidbodyComponent::Init
(
	Maths::Vec3 _size,
	Maths::Vec3 _position,
	Maths::Quat _rotation
) {
	p_rigidbody_ = std::make_shared<Physics::SphereRigidbody>
		(std::max(sphereRadius_ * _size.GetMagnitude(), .2),
			_position + centerOffset_,
			_rotation * rotationOffset_,
			motionType_,
			bMovementAtStart_);

	rotationParent = _rotation;
	center = _position;
}

double ECS::Components::SphereRigidbodyComponent::GetRadius() const
{
	return std::dynamic_pointer_cast<Physics::SphereRigidbody>(p_rigidbody_)->GetRadius();
}

void ECS::Components::SphereRigidbodyComponent::SetRadius(double _newRadius)
{
	std::dynamic_pointer_cast<Physics::SphereRigidbody>(p_rigidbody_)->SetBounds(std::max(_newRadius, .15));
}