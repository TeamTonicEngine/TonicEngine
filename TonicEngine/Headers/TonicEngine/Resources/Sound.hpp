#pragma once
#include <TonicEngine/DLL_API.hpp>
#include "IResource.hpp"


namespace Resources
{

	class TONIC_ENGINE_API Sound : public IResource
	{
	protected:
		
		std::filesystem::path p_soundPath_;
	public:
		

		Sound(std::filesystem::path const _soundPath);
		~Sound();
		void Init();
		void LoadSound();
		void Play(const float& _millisecond = 0.f);
		void Pause();
		void Stop();
		void SetLoop(const bool& _bLoop);
		void SetVolume(const float& volume);


		std::string ReadFile(std::filesystem::path const _path) override;
		void MetaWriteFile(const string _name) override;
		void MetaReadFile(const string _name) override;

		void ResourceUnload() override;
	};

}