#include "pch.hpp"
#include "ECS/Components/AudioSourceComponent.hpp"
#include "Resources/Sound.hpp"

ECS::Components::AudioSourceComponent::AudioSourceComponent(Resources::SoundPtr _p_sound)
{
	SetSound(_p_sound);
}

void ECS::Components::AudioSourceComponent::SetVolume(float _volume)
{
	volume_ = _volume;
	if (p_sound_)
		p_sound_->SetVolume(_volume);
	else
		DEBUG_ERROR("NO SOUND");
}

void ECS::Components::AudioSourceComponent::SetPitch(float _pitch)
{
	pitch_ = _pitch;
	if (p_sound_)
		p_sound_->SetPitch(_pitch);
}

void ECS::Components::AudioSourceComponent::SetLoop(bool _bLoop)
{
	bLoop_ = _bLoop;
	if (p_sound_)
		p_sound_->SetLoop(_bLoop);
	else
		DEBUG_ERROR("CANNOT LOOP THIS SOUND");
}

void ECS::Components::AudioSourceComponent::SetIsSpatialized(bool _bSpatialized)
{
	bSpatialized_ = _bSpatialized;
	if (p_sound_)
		p_sound_->SetIsSpatialized(_bSpatialized);
}

void ECS::Components::AudioSourceComponent::SetPlayOnStart(bool _bPlayOnStart)
{
	bPlayOnStart_ = _bPlayOnStart;
}

void ECS::Components::AudioSourceComponent::SetSound(Resources::SoundPtr _p_sound)
{
	p_sound_ = _p_sound;
	if (p_sound_)
	{
		SetIsSpatialized(bSpatialized_);
		SetLoop(bLoop_);
		SetVolume(volume_);
		SetPitch(pitch_);
	}
}

void ECS::Components::AudioSourceComponent::Play()
{
	if (!p_sound_)
		DEBUG_WARNING("No sound to play")
	else
		p_sound_->Play();
}

void ECS::Components::AudioSourceComponent::SetPosition(const Maths::Vec3& _position)
{
	if (p_sound_)
		p_sound_->SetPosition(_position);
}
void ECS::Components::AudioSourceComponent::SetVelocity(const Maths::Vec3& _velocity)
{
	if (p_sound_)
		p_sound_->SetVelocity(_velocity);
}