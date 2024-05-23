#include "pch.hpp"

#include "Resources/Shader.hpp"

#include "Core/Engine.hpp"

Resources::Shader::Shader() { type_ = ResourceType::Shader; }

void Resources::Shader::Destroy() { ENGINE.RDR->UnloadResource(shared_from_this()); }

void Resources::Shader::ReadFile(const fs::path _path)
{
	std::string name = _path.filename().replace_extension("").string();
	SetFragment(name);
	SetVertex(name);
}

void Resources::Shader::LoadFile()
{
	if (bRead_)
	{
		ENGINE.RDR->LoadResource(shared_from_this());
		if (bLoaded_)
			DEBUG_SUCCESS("Shader program %s created #%i", name.c_str(), ID)
		else
			DEBUG_WARNING("Loading failed for Shader %s", name.c_str());
	}
	else
		DEBUG_WARNING("File not Read, cannot be loaded: %s", name.c_str());
}

void Resources::Shader::SetVertex(const std::string _name)
{
	fs::path filePathVert = FindFile(_name + ".vert", type_);
	if (filePathVert.empty()) // If no vertex shader found
	{
		DEBUG_LOG("Shader: VERTEX file not found for %s", _name.c_str());
		DEBUG_LOG("Shader class uses .vert & .frag extensions");
		return;
	}
	else // Found
	{
		vertexShaderPath_ = filePathVert;
		vertexCode_ = Resources::ReadFileContent(vertexShaderPath_);

		if (fragmentCode_ != "")
			bRead_ = true;
	}
}

void Resources::Shader::SetFragment(const std::string _name)
{
	fs::path filePathFrag = FindFile(_name + ".frag", type_);
	if (filePathFrag.empty()) // If no fragment shader found
	{
		DEBUG_LOG("Shader: FRAGMENT file not found for %s", _name.c_str());
		DEBUG_LOG("Shader class uses .vert & .frag extensions");
		return;
	}
	else // Found
	{
		fragmentShaderPath_ = filePathFrag;
		fragmentCode_ = Resources::ReadFileContent(fragmentShaderPath_);

		if (vertexCode_ != "")
			bRead_ = true;
	}
}

void Resources::Shader::Use()
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

void Resources::Shader::StopUse() const { ENGINE.RDR->StopUseShader(); }

fs::path Resources::Shader::GetResourcePath() const
{
	DEBUG_WARNING("Shader Class has no single Path, use a more specific Getter");
	return fs::path();
}

fs::path Resources::Shader::GetVertexPath() const { return vertexShaderPath_; }

fs::path Resources::Shader::GetFragmentPath() const { return fragmentShaderPath_; }