#pragma once

#include "DLL_API.hpp"

#include "IResource.hpp"

class ma_decoder;
class ma_device;
class ma_spatializer;
class ma_spatializer_listener;

namespace Components
{
	struct AudioListenerComponent;
}
namespace Resources
{
	class Sound : public IResource
	{
	public:
		float pitch_ = 1.0f;
		ma_decoder* decoder;
		ma_device* device;
		ma_spatializer* spatializer;
		ma_spatializer_listener* listener;
		bool isSpatialized = false;

		const char* name_ = "None";

		TONIC_ENGINE_API Sound();
		TONIC_ENGINE_API ~Sound();

		void Destroy() override;

		void TONIC_ENGINE_API LoadSound();
		void TONIC_ENGINE_API Restart();
		void TONIC_ENGINE_API Play();
		void TONIC_ENGINE_API Pause();
		bool TONIC_ENGINE_API IsFinished() const;
		void TONIC_ENGINE_API Stop();
		void TONIC_ENGINE_API SetLoop(const bool& _bLoop);
		void TONIC_ENGINE_API SetVolume(const float& _volume);
		void TONIC_ENGINE_API InitSpatialization();
		float TONIC_ENGINE_API GetOggFileDuration();

		void TONIC_ENGINE_API SetPitch(const float& _pitch);
		void TONIC_ENGINE_API SetIsSpatialized(const bool& _enabled);
		void TONIC_ENGINE_API SetPosition(const Maths::Vec3& _position);
		void TONIC_ENGINE_API SetVelocity(const Maths::Vec3& _velocity);

		void TONIC_ENGINE_API ReadFile(const fs::path _path) override;
		void TONIC_ENGINE_API LoadFile() override {};

		void TONIC_ENGINE_API MetaWriteFile(const string _name) override {};
		void TONIC_ENGINE_API MetaReadFile(const string _name) override {};

		void TONIC_ENGINE_API ResourceUnload() override {};
		void TONIC_ENGINE_API Use() override {};

		friend struct Components::AudioListenerComponent;
	};
	using SoundPtr = std::shared_ptr<Sound>;
}
