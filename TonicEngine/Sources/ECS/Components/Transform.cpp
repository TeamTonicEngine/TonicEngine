#include "pch.hpp"

#include "ECS/Components/Transform.hpp"

Components::Transform::Transform(const Maths::Vec3& _position, Maths::Quat _rotation, Maths::Vec3 _scale)
	: position(_position), rotation(_rotation), scale(_scale) {};

Components::Transform::Transform(const Maths::Vec3& _position, Maths::Vec3 _rotationEuler, Maths::Vec3  _scale)
	: position(_position), rotation(Maths::Quaternions::FromEulerAngles(_rotationEuler)), scale(_scale) {};

Components::Transform::Transform(const Maths::Vec3 & _position, float _rotationAngle, Maths::Vec3 _rotationAxis, Maths::Vec3  _scale)
	: position(_position), rotation(Maths::Quaternions::FromAngleAxis(_rotationAngle, _rotationAxis)), scale(_scale) {};

Maths::Mat4 Components::Transform::GetModel()
{
	return Maths::Mat4::Transform(position, rotation, scale);
}

void Components::Transform::SetModelUniform()
{
	Core::Renderer::RHI* p_rhi = HEART::GetRenderer();
	p_rhi->SetUniform("Model", GetModel());
}