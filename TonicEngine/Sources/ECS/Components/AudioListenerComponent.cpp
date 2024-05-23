#include "pch.hpp"

#include "ECS/Components/AudioListenerComponent.hpp"

#include "Audio/miniaudio.h"

u32 ECS::Components::AudioListenerComponent::s_ids_ = 0;

void ECS::Components::AudioListenerComponent::Init()
{
	id_ = s_ids_++;
	Maths::Vec3 direction = (Maths::Vec3::FORWARD);
	ma_engine_listener_set_direction(ENGINE.AUD_ENG, id_, 0.f, 0.f, 1.f);
	ma_engine_listener_set_enabled(ENGINE.AUD_ENG, id_, true);
	ma_engine_listener_set_position(ENGINE.AUD_ENG, id_, 0.f, 0.f, 0.f);
	ma_engine_listener_set_world_up(ENGINE.AUD_ENG, id_, 0.f, 1.f, 0.f);
}

void ECS::Components::AudioListenerComponent::SetPosition(Maths::Vec3 _position)
{
	ma_engine_listener_set_position(ENGINE.AUD_ENG, id_, _position.x, _position.y, _position.z);
}

void ECS::Components::AudioListenerComponent::SetDirection(Maths::Quat _rotation)
{
	Maths::Vec3 direction = _rotation.RotateVector(Maths::Vec3::FORWARD);
	ma_engine_listener_set_direction(ENGINE.AUD_ENG, id_, direction.x, direction.y, direction.z);
}

void ECS::Components::AudioListenerComponent::SetCone(float _innerAngleRad, float _outerAngleRad, float _outerGain)
{
	ma_engine_listener_set_cone(ENGINE.AUD_ENG, id_, _innerAngleRad, _outerAngleRad, _outerGain);
}

void ECS::Components::AudioListenerComponent::SetVelocity(Maths::Vec3 _velocity)
{
	ma_engine_listener_set_velocity(ENGINE.AUD_ENG, id_, _velocity.x, _velocity.y, _velocity.z);
}

void ECS::Components::AudioListenerComponent::SetEnabled(bool _value)
{
	bEnabled_ = _value;
	ma_engine_listener_set_enabled(ENGINE.AUD_ENG, id_, _value);
}
;