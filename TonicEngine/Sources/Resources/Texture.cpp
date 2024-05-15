#include "pch.hpp"

#include "Resources\Texture.hpp"

//#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.hpp"

Resources::TexturePtr Resources::Texture::_s_p_defaultTexture;

Resources::Texture::Texture(){type_ = ResourceType::Texture;}


void Resources::Texture::Destroy()
{
	ENGINE.RDR->UnloadResource(shared_from_this());
	//IMPORTANT: this line prevents memory leaks: Case read but never loaded so never freed
	if (bRead_ && !bLoaded_)
		stbi_image_free(data_);
}

void Resources::Texture::ReadFile(const fs::path _path)
{
	if (bLoaded_)
		return;
	if (data_)
		stbi_image_free(data_);

	stbi_set_flip_vertically_on_load(true);
	data_ = stbi_load(_path.string().c_str(), &width_, &height_, &nrComponents_, 0);
	if (data_)
		bRead_ = true;
	else
	{
		DEBUG_ERROR("Texture failed read at path: %s", _path.string().c_str());
		stbi_image_free(data_);
	}
}

void Resources::Texture::LoadFile()
{
	//if (!bRead_)
	//	ReadFile(path);

	if (bRead_)
	{
		ENGINE.RDR->LoadResource(shared_from_this());
		if (bLoaded_)
		{
			DEBUG_SUCCESS("Texture %s loaded #%i", name.c_str(), ID);
			bLoaded_ = true;

			stbi_image_free(data_);
			bRead_ = false;
		}
		else
			DEBUG_WARNING("Loading failed for Texture %s", name.c_str());
	}
	else
		DEBUG_WARNING("File not Read, cannot be loaded: %s", name.c_str());

	if (!_s_p_defaultTexture)
		_s_p_defaultTexture = shared_from_this();
}

void Resources::Texture::LoadFileForced()
{
	do { LoadFile(); } while (!bLoaded_);
}

void Resources::Texture::Use()
{
	if (this)
	{
		if (bLoaded_)
			ENGINE.RDR->UseResource(shared_from_this());
		else
		{
			LoadFile();
			if (bLoaded_)
				ENGINE.RDR->UseResource(shared_from_this());
		}
	}
	else
		ENGINE.RDR->UseResource(_s_p_defaultTexture);
}

void Resources::Texture::Use(Resources::TextureType _type)
{
	if (this)
	{
		if (bLoaded_)
			ENGINE.RDR->UseResource(shared_from_this(),_type);
		else
		{
			LoadFile();
			if (bLoaded_)
				ENGINE.RDR->UseResource(shared_from_this(),_type);
		}
	}
	else
		ENGINE.RDR->UseResource(_s_p_defaultTexture, _type);
}
