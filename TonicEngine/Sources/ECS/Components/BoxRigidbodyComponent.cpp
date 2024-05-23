#include "pch.hpp"

#include "Physics/BoxRigidbody.hpp"
#include "ECS/Components/BoxRigidbodyComponent.hpp"

template TONIC_ENGINE_API const ECS::ComponentTypeID ECS::CompType<ECS::Components::BoxRigidbodyComponent>() noexcept;

ECS::Components::BoxRigidbodyComponent::BoxRigidbodyComponent()
	: BoxRigidbodyComponent(Maths::Vec3(1), Maths::Vec3(0))
{}

ECS::Components::BoxRigidbodyComponent::BoxRigidbodyComponent
(
	Maths::Vec3 _boxExtents,
	Maths::Vec3 _boxPosition,
	Maths::Quat _boxRotation, /* = identity */
	Physics::Motions::MotionType _motionType, /* = dynamic */
	bool _bActivateMovementAtCreation /* = true */
) {
	boxExtents_ = _boxExtents;
	centerOffset_ = _boxPosition;
	rotationOffset_ = _boxRotation;
	motionType_ = _motionType;
	bMovementAtStart_ = _bActivateMovementAtCreation;
}

ECS::Components::BoxRigidbodyComponent::~BoxRigidbodyComponent() { DestroyRigidbody(); }

void ECS::Components::BoxRigidbodyComponent::Init
(
	Maths::Vec3 _size,
	Maths::Vec3 _position,
	Maths::Quat _rotation
) {
	p_rigidbody_ = std::make_shared<Physics::BoxRigidbody>
		(Maths::Vec3(boxExtents_.x * _size.x, boxExtents_.y * _size.y, boxExtents_.z * _size.z),
			_position + centerOffset_,
			_rotation,
			motionType_,
			bMovementAtStart_);

	rotationParent = _rotation;
	center = _position;
}

Maths::Vec3 ECS::Components::BoxRigidbodyComponent::GetExtents() const
{
	return std::dynamic_pointer_cast<Physics::BoxRigidbody>(p_rigidbody_)->GetBounds();
}

void ECS::Components::BoxRigidbodyComponent::SetExtents(Maths::Vec3 _newExtents)
{
	std::dynamic_pointer_cast<Physics::BoxRigidbody>(p_rigidbody_)->SetBounds(_newExtents);
}