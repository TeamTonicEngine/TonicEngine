#include "pch.hpp"

#include "Resources\Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.hpp"

Resources::Texture::Texture() { type_ = ResourceType::Texture; }

Resources::Texture::~Texture() { GetRDR()->UnloadTexture(this); }

void Resources::Texture::ReadFile(const string _name)
{
	if (!path.empty())
		ReadFile(path);
	else
	{
		auto path = FindFile(_name, type_);
		ReadFile(path);
	}
}

void Resources::Texture::ReadFile(const fs::path _path)
{
	stbi_set_flip_vertically_on_load(true);
	data_ = stbi_load(_path.string().c_str(), &width_, &height_, &nrComponents_, 0);
	if (data_)
	{
		GetRDR()->LoadTexture(this);
		stbi_image_free(data_);
	}
	else
	{
		DEBUG_WARNING("Texture failed to load at path: %s", _path.c_str());
		stbi_image_free(data_);
	}
}

void Resources::Texture::SetTextureType(const TextureType _newType) { texType_ = _newType; }

const std::string Resources::Texture::GetTextureType() { return TextureTypeToString(texType_); }

void Resources::Texture::Use() const { GetRDR()->UseTexture(this); }