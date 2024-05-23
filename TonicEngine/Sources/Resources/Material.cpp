#include "pch.hpp"

#include "Resources/Material.hpp"
#include "Resources/Shader.hpp"

Resources::Material::Material()
	: type_(Materials::MaterialType::Unset)
{
	bRead_ = bLoaded_ = true; /* Not needed for now */
}

Resources::Material::Material(TexturePtr _diffuse)
	: diffuse_(_diffuse), type_(Materials::MaterialType::Unlit)
{
	p_shader_ = ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\unlit.frag");
	bRead_ = bLoaded_ = true; /* Not needed for now */
}

Resources::Material::Material(TexturePtr _diffuse,
	TexturePtr _specular,
	float _shininess,
	float _gammaCorrection
)
	: diffuse_(_diffuse), specular_(_specular), shininess_(_shininess), gammaCorrection_(_gammaCorrection), type_(Materials::MaterialType::Phong)
{
	p_shader_ = ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\phong.frag");
	bRead_ = bLoaded_ = true; /* Not needed for now */
}

Resources::Material::Material(TexturePtr _diffuse,
	TexturePtr _specular,
	TexturePtr _ao,
	TexturePtr _normal,
	TexturePtr _roughness,
	float _gammaCorrection
)
	: diffuse_(_diffuse), specular_(_specular), ao_(_ao), normal_(_normal), roughness_(_roughness), gammaCorrection_(_gammaCorrection), type_(Materials::MaterialType::PBR)
{
	p_shader_ = ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\pbr.frag");

	bRead_ = bLoaded_ = true; /* Not needed for now */
}

void Resources::Material::SetUniform(Resources::ShaderPtr _shader)
{
	_shader->Use();
	Use();
}

void Resources::Material::Use() { ENGINE.RDR->UseResource(shared_from_this()); }

void Resources::Material::SetType(Resources::Materials::MaterialType _type)
{
	type_ = _type;
	switch (_type)
	{
	case Materials::MaterialType::Unlit:
		p_shader_ = ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\unlit.frag");
		break;
	case Materials::MaterialType::Phong:
		p_shader_ = ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\phong.frag");
		break;
	case Materials::MaterialType::PBR:
		p_shader_ = ENGINE.RES_MNGR->Get<Resources::Shader>("Assets\\Shaders\\pbr.frag");
		break;
	default:
		break;
	}
}