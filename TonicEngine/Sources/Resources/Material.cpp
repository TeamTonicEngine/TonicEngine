#include "pch.hpp"
#include "Resources/Material.hpp"

Resources::Material::Material(Maths::Vec3 _albedo, float _metallic, float _roughness, float _ao) : albedo_(_albedo), metallic_(_metallic), roughness_(_roughness), ao_(_ao)
{
};

void Resources::Material::SetUniform(Shader* _shader)
{
	Core::Renderer::RHI* p_rhi = HEART::GetRenderer();

	p_rhi->UseShader(_shader);
	p_rhi->SetUniform("material.Albedo", albedo_);
	p_rhi->SetUniform("material.Ao", ao_);
	p_rhi->SetUniform("material.Metallic", metallic_);
	p_rhi->SetUniform("material.Roughness", roughness_);

	//////////////////////////////////////////////////////////////////////
	//TODO: Uncomment after the creation of SetSample2D in Shader
	p_rhi->SetUniform("AlbedoMap", map_.albedo, 1);
	p_rhi->SetUniform("AoMap", map_.ao, 0);
	p_rhi->SetUniform("NormalMap", map_.normal, 2);
	p_rhi->SetUniform("MetallicMap", map_.metallic, 3);
	p_rhi->SetUniform("RoughnessMap", map_.roughness, 4);
	//////////////////////////////////////////////////////////////////////
}

void Resources::Material::ReadFile(const string _name)
{
}

void Resources::Material::ReadFile(const fs::path _path)
{
}

void Resources::Material::MetaWriteFile(const string _name)
{
}

void Resources::Material::MetaReadFile(const string _name)
{
}

void Resources::Material::ResourceUnload()
{
}