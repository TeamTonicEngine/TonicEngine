#pragma once

#include "DLL_API.hpp"

#include "IResource.hpp"
#include "Resources/Shader.hpp"
#include "Resources/Texture.hpp"

namespace Core::Renderer { class OpenGLWrapper; }

namespace Resources
{
	namespace Materials
	{
		enum class TONIC_ENGINE_API MaterialType
		{
			Unset = -1,
			Unlit,
			Phong,
			PBR
		};

		static const std::string MaterialTypeToString(const MaterialType& _type)
		{
			switch (_type)
			{
			case MaterialType::Unlit:
				return "Unlit";
			case MaterialType::Phong:
				return "Phong";
			case MaterialType::PBR:
				return "PBR";
			case MaterialType::Unset:
			default:
				return "Unset";
			}
		}
	}

	class Material : public IResource, public std::enable_shared_from_this<Material>
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		ShaderPtr p_shader_;

		TexturePtr diffuse_;
		TexturePtr specular_;
		TexturePtr ao_;
		TexturePtr normal_;
		TexturePtr roughness_;
		float shininess_ = 32.f;

		float gammaCorrection_ = 2.2f;
		Materials::MaterialType type_ = Materials::MaterialType::Unset;

	public:
		_declspec(property(get = GetType, put = SetType))
			Materials::MaterialType type;

		_declspec(property(get = GetDiffuse, put = SetDiffuse))
			TexturePtr diffuse;

		_declspec(property(get = GetSpecular, put = SetSpecular))
			TexturePtr specular;

		_declspec(property(get = GetAo, put = SetAo))
			TexturePtr ao;

		_declspec(property(get = GetNormal, put = SetNormal))
			TexturePtr normal;

		_declspec(property(get = GetRoughness, put = SetRoughness))
			TexturePtr roughness;

		_declspec(property(get = GetShininess, put = SetShininess))
			float shininess;

		_declspec(property(get = GetGammaCorrection, put = SetGammaCorrection))
			float gammaCorrection;

		friend class Core::Renderer::OpenGLWrapper;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		//Unset
		TONIC_ENGINE_API Material();
		//Unlit
		TONIC_ENGINE_API Material(TexturePtr _diffuse);
		//Phong
		TONIC_ENGINE_API Material(TexturePtr _diffuse,
			TexturePtr _specular,
			float _shininess = 32.f,
			float _gammaCorrection = 2.2f);
		//PBR
		TONIC_ENGINE_API Material(TexturePtr _diffuse,
			TexturePtr _specular,
			TexturePtr _ao,
			TexturePtr _normal,
			TexturePtr _roughness,
			float _gammaCorrection = 2.2f);

		TONIC_ENGINE_API ~Material() = default;

		void Destroy() override {};

		void Use() override;
		void TONIC_ENGINE_API SetUniform(Resources::ShaderPtr _shader);

		void ReadFile(const fs::path _path) override {};

		void LoadFile() override {};
		void ResourceUnload() override {};

		inline bool IsTransparent() { return diffuse_ ? diffuse_->IsTransparent() : false; }
		inline ShaderPtr GetShader() const { return p_shader_; }

		inline Materials::MaterialType GetType() const { return type_; };
		inline void TONIC_ENGINE_API SetType(Materials::MaterialType _type);

		inline TexturePtr GetDiffuse() const { return diffuse_; };
		inline void SetDiffuse(TexturePtr _diffuse) { diffuse_ = _diffuse; };

		inline TexturePtr GetSpecular() const { return specular_; };
		inline void SetSpecular(TexturePtr _specular) { specular_ = _specular; };

		inline TexturePtr GetAo() const { return ao_; };
		inline void SetAo(TexturePtr _specular) { ao_ = _specular; };

		inline TexturePtr GetNormal() const { return normal_; };
		inline void SetNormal(TexturePtr _normal) { normal_ = _normal; };

		inline TexturePtr GetRoughness() const { return roughness_; };
		inline void SetRoughness(TexturePtr _roughness) { roughness_ = _roughness; };

		inline float GetShininess() const { return shininess_; };
		inline void SetShininess(float _shininess) { shininess_ = _shininess; };

		inline float GetGammaCorrection() const { return gammaCorrection_; };
		inline void SetGammaCorrection(float _gammaCorrection) { gammaCorrection_ = _gammaCorrection; };
	};
	using MaterialPtr = std::shared_ptr<Material>;
}