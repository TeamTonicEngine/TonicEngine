#pragma once

#include "DLL_API.hpp"

#include "IResource.hpp"

namespace Resources
{
	class Sound : public IResource
	{
	protected:
	public:
		TONIC_ENGINE_API Sound();
		TONIC_ENGINE_API ~Sound();
		void TONIC_ENGINE_API LoadSound();
		void TONIC_ENGINE_API Play(const float& _millisecond = 0.f);
		void TONIC_ENGINE_API Pause();
		void TONIC_ENGINE_API Stop();
		void TONIC_ENGINE_API SetLoop(const bool& _bLoop);
		void TONIC_ENGINE_API SetVolume(const float& volume);

		void TONIC_ENGINE_API ReadFile(const string _name) override;
		void TONIC_ENGINE_API ReadFile(const fs::path _path) override;

		void TONIC_ENGINE_API MetaWriteFile(const string _name) override {};
		void TONIC_ENGINE_API MetaReadFile(const string _name) override {};

		void TONIC_ENGINE_API ResourceUnload() override {};
		void TONIC_ENGINE_API Use() const override {};
	};
}