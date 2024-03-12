#include "pch.hpp"
#include "TonicEngine/Resources/Texture.hpp"

Resources::Texture::Texture(std::filesystem::path const _texturePath, std::string _fileName)
{
	texturePath_ = _texturePath;
	name = _fileName;

}

Resources::Texture::~Texture()
{

}

std::string Resources::Texture::ReadFile(std::filesystem::path const _path)
{
	return "";
}

void Resources::Texture::MetaWriteFile(const string _name)
{

}

void Resources::Texture::MetaReadFile(const string _name)
{

}

void Resources::Texture::ResourceUnload()
{

}