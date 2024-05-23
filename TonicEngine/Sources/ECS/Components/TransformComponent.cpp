#include "pch.hpp"

#include "ECS/Components/TransformComponent.hpp"

ECS::Components::TransformComponent::TransformComponent(const Maths::Vec3& _position, Maths::Quat _rotation, Maths::Vec3 _scale)
	: position_(_position), rotation_(_rotation), scale_(_scale), bChanged_(true)
{}

ECS::Components::TransformComponent::TransformComponent(const Maths::Vec3& _position, Maths::Vec3 _rotationEuler, Maths::Vec3  _scale)
	: position_(_position), rotation_(Maths::Quaternions::FromEulerAngles(_rotationEuler)), scale_(_scale), bChanged_(true)
{}

ECS::Components::TransformComponent::TransformComponent(const Maths::Vec3& _position, float _rotationAngle, Maths::Vec3 _rotationAxis, Maths::Vec3  _scale)
	: position_(_position), rotation_(Maths::Quaternions::FromAngleAxis(_rotationAngle, _rotationAxis)), scale_(_scale), bChanged_(true)
{}

bool ECS::Components::TransformComponent::HasChanged() const { return bChanged_; }

const Maths::Vec3 ECS::Components::TransformComponent::GetPosition() const { return position_; }

const Maths::Vec3 ECS::Components::TransformComponent::GetLocalPosition() const
{
	auto p_em = ENGINE.ENT_MNGR;
	auto parent = p_em->GetParent(this->entityID_);
	while (parent)
	{
		if (p_em->HasComponent<ECS::Components::TransformComponent>(parent))
			return position_ - p_em->GetComponent<ECS::Components::TransformComponent>(parent).position;
		parent = p_em->GetParent(parent);
	}
	return position_;
}

const Maths::Vec3 ECS::Components::TransformComponent::GetRotationFromEuler() const
{
	return rotation_.ToEulerAngles() * Maths::Constants::RAD2DEG;
}

const Maths::Quat ECS::Components::TransformComponent::GetRotation() const { return rotation_; }

const Maths::Vec3 ECS::Components::TransformComponent::GetScale() const { return scale_; }

void ECS::Components::TransformComponent::SetPosition(Maths::Vec3 _newPosition)
{
	positionDiff_ = position_ - _newPosition;
	position_ = _newPosition;
	bChanged_ = true;
}

void ECS::Components::TransformComponent::SetLocalPosition(Maths::Vec3 _newPosition)
{
	auto p_em = ENGINE.ENT_MNGR;
	auto parent = p_em->GetParent(this->entityID_);
	while (parent)
	{
		if (p_em->HasComponent<ECS::Components::TransformComponent>(parent))
		{
			SetPosition(_newPosition + p_em->GetComponent<ECS::Components::TransformComponent>(parent).position);
			return;
		}
		parent = p_em->GetParent(parent);
	}
	return SetPosition(_newPosition);
}

//In Degrees not radians
void ECS::Components::TransformComponent::SetRotationFromEuler(Maths::Vec3 _newRotation)
{
	SetRotation(Maths::Quaternions::FromEulerAngles(_newRotation * Maths::Constants::DEG2RAD));
}

void ECS::Components::TransformComponent::SetRotation(Maths::Quat _newRotation)
{
	rotationDiff_ = rotation_.GetInverse() * _newRotation;
	rotation_ = _newRotation;
	bChanged_ = true;
}

void ECS::Components::TransformComponent::SetScale(Maths::Vec3 _newScale)
{
	for (int i = 0; i < 3; i++)
		if (_newScale[i] == 0.f)
			*(&_newScale.x + i) = 0.00001f;
	scaleDiff_ = Maths::Vec3(_newScale.x / scale_.x, _newScale.y / scale_.y, _newScale.z / scale_.z);
	scale_ = _newScale;
	bChanged_ = true;
}

void ECS::Components::TransformComponent::LookAt(Maths::Vec3 _pointOfInterest)
{
	SetRotation(Maths::Quaternions::LookAt(position_, _pointOfInterest));
}

const Maths::Mat4 ECS::Components::TransformComponent::GetModel() const
{
	return Maths::Mat4::Transform(position_, rotation_, scale_);
}

void ECS::Components::TransformComponent::ResetChanged()
{
	//Reset Flags
	if (bChanged_)
	{
		positionDiff_ = { 0.f };
		rotationDiff_ = Maths::Quat::Identity();
		scaleDiff_ = { 1.f };
		bChanged_ = false;
	}
}