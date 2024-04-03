#pragma once

#include "DLL_API.hpp"

#include "IResource.hpp"

namespace Core::Renderer { class OpenGLWrapper; }; // Needed for friend class

namespace Resources
{
	enum class TONIC_ENGINE_API TextureType
	{
		Unset,
		Diffuse,
		Specular,
		Normal,
		AO,
		Roughness,
	};

	static const string TextureTypeToString(const TextureType& type)
	{
		switch (type)
		{
		case TextureType::Diffuse:
			return "texture_diffuse";
		case TextureType::Specular:
			return "texture_specular";
		case TextureType::Normal:
			return "texture_normal";
		case TextureType::AO:
			return "texture_ao";
		case TextureType::Roughness:
			return "texture_roughness";

		case TextureType::Unset:
		default:
			return "";
		}
	}

	class Texture : public IResource
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		int width_ = -1, height_ = -1, nrComponents_ = -1;
		unsigned char* data_ = nullptr;
		TextureType texType_ = TextureType::Unset;

		friend class Core::Renderer::OpenGLWrapper;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		TONIC_ENGINE_API Texture();
		TONIC_ENGINE_API ~Texture();

		void TONIC_ENGINE_API ReadFile(const string _name) override;
		void TONIC_ENGINE_API ReadFile(const fs::path _path) override;

		void SetTextureType(const TextureType _newType);
		const string GetTextureType();

		void TONIC_ENGINE_API MetaWriteFile(const string _name) override {};
		void TONIC_ENGINE_API MetaReadFile(const string _name) override {};

		void TONIC_ENGINE_API ResourceUnload() override {};

		void TONIC_ENGINE_API Use() const override;
	};
}