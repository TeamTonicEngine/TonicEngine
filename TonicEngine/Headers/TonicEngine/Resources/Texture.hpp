#pragma once
#include <TonicEngine/DLL_API.hpp>
#include "IResource.hpp"

namespace Resources 
{
	class TONIC_ENGINE_API Texture : public IResource
	{
	private:
		ResourceType type = ResourceType::Texture;
	public:
		std::string name;
		unsigned int ID = 0;
		std::filesystem::path texturePath_ = std::filesystem::path();

		Texture(std::filesystem::path const _texturePath, std::string _fileName);
		~Texture();

		std::string ReadFile(std::filesystem::path const _path) override;
		void MetaWriteFile(const string _name) override;
		void MetaReadFile(const string _name) override;

		void ResourceUnload() override;
	};
}


