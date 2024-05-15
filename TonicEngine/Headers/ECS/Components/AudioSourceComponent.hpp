#pragma once

#include "DLL_API.hpp"

#include "ECS/Base/BaseComponent.hpp"
#include "LowRenderer/Cameras/Camera.hpp"
#include "Maths/Maths.hpp"
namespace ECS::Systems
{
	class AudioSourceSystem;
}
namespace Resources
{
	class Sound;
}

namespace ECS::Components
{
	struct AudioSourceComponent : public ECS::BaseComponent
	{
	public:
		TONIC_ENGINE_API AudioSourceComponent() = default;
		TONIC_ENGINE_API ~AudioSourceComponent() = default;
		TONIC_ENGINE_API AudioSourceComponent(const AudioSourceComponent&) = default;
		TONIC_ENGINE_API AudioSourceComponent(Resources::SoundPtr _p_sound);

		void TONIC_ENGINE_API SetVolume(float _volume);
		void TONIC_ENGINE_API SetPitch(float _pitch);
		void TONIC_ENGINE_API SetLoop(bool _bLoop);
		void TONIC_ENGINE_API SetIsSpatialized(bool _bSpatialized);
		void TONIC_ENGINE_API SetPlayOnStart(bool _bPlayOnStart);

		void TONIC_ENGINE_API SetSound(Resources::SoundPtr _p_sound);
		void TONIC_ENGINE_API Play();

		friend class ECS::Systems::AudioSourceSystem;

		__declspec(property(get = GetVolume, put = SetVolume))
			float volume;
		inline float GetVolume() const { return volume_; };


		__declspec(property(get = GetPitch, put = SetPitch))
			float pitch;
		inline float GetPitch() const { return pitch_; };


		__declspec(property(get = GetSpatialized, put = SetIsSpatialized))
			bool bSpatialized;
		inline bool GetSpatialized() const { return bSpatialized_; };


		__declspec(property(get = GetLoop, put = SetLoop))
			bool bLoop;
		inline bool GetLoop() const { return bLoop_; };


		__declspec(property(get = GetPlayOnStart, put = SetPlayOnStart))
			bool bPlayOnStart;
		inline bool GetPlayOnStart() const { return bPlayOnStart_; };


		__declspec(property(get = GetSound, put = SetSound))
			Resources::SoundPtr p_sound;
		inline Resources::SoundPtr GetSound() const { return p_sound_; };;


	protected:
		void TONIC_ENGINE_API SetPosition(const Maths::Vec3& _position);
		void TONIC_ENGINE_API SetVelocity(const Maths::Vec3& _velocity);

		float volume_ = 0.7f;
		float pitch_ = 1.f;

		bool bSpatialized_ = false;
		bool bLoop_ = false;
		bool bPlayOnStart_ = true;
		Resources::SoundPtr p_sound_ = nullptr;
	};
}