#pragma once

#include "DLL_API.hpp"

#include "IResource.hpp"

namespace Core::Renderer { class OpenGLWrapper; }; // Needed for friend class

namespace Resources
{
	enum class TONIC_ENGINE_API TextureType
	{
		Unset = -1, //Don't change this or Modify the UseTexture for "glActiveTexture(GL_TEXTURE0 + (int)_p_texture->textureType -1); "
		Diffuse, // == Albedo
		Specular, // == metallic
		Normal,
		AO,
		Roughness,
		CubeMap = 0x8513/*GL_TEXTURE_CUBE_MAP*/ - 0x84C0/*GL_TEXTURE_0*/
	};

	static const string TextureTypeToString(const TextureType& _type)
	{
		switch (_type)
		{
		case TextureType::Diffuse:
			return "material.diffuse";
		case TextureType::Specular:
			return "material.specular";
		case TextureType::Normal:
			return "material.normal";
		case TextureType::AO:
			return "material.ao";
		case TextureType::Roughness:
			return "material.roughness";

		case TextureType::Unset:
		default:
			return "";
		}
	}
	class Texture;

	using TexturePtr = std::shared_ptr<Texture>;

	class Texture : public IResource, public std::enable_shared_from_this<Texture>
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		int width_ = -1, height_ = -1, nrComponents_ = -1;
		unsigned char* data_ = nullptr;

	public:
		TextureType textureType = TextureType::Unset;

		static TexturePtr _s_p_defaultTexture;

		friend class Core::Renderer::OpenGLWrapper;
		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		TONIC_ENGINE_API Texture();
		TONIC_ENGINE_API ~Texture() = default;

		void TONIC_ENGINE_API Destroy() override;

		void ReadFile(const fs::path _path) override;
		void LoadFile() override;
		void TONIC_ENGINE_API LoadFileForced() override;

		void TONIC_ENGINE_API MetaWriteFile(const string _name) override {};
		void TONIC_ENGINE_API MetaReadFile(const string _name) override {};

		void TONIC_ENGINE_API ResourceUnload() override {};

		void TONIC_ENGINE_API Use() override;
		//Overrides the TextureType
		void TONIC_ENGINE_API Use(Resources::TextureType _type);
		bool IsTransparent() { return nrComponents_ == 4; }
	};
}