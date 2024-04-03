#include "pch.hpp"

#include "Resources/Shader.hpp"

#include "Core/Engine.hpp"

Resources::Shader::Shader() { type_ = ResourceType::Shader; }

Resources::Shader::~Shader() { GetRDR()->UnloadShader(this); }

void Resources::Shader::ReadFile(const string _name)
{
	SetFragment(_name, false);
	SetVertex(_name);
}

void Resources::Shader::ReadFile(const fs::path _path)
{
	string name = _path.filename().replace_extension("").string();

	SetFragment(name, false);
	SetVertex(name);
}

void Resources::Shader::SetVertex(const string _name, bool _autoLinkRHI)
{
	fs::path filePathVert = FindFile(_name + ".vert", type_);
	if (filePathVert.empty()) // If no vertex shader found
	{
		DEBUG_LOG("Shader: VERTEX file not found for %s", _name.c_str());
		DEBUG_LOG("Shader class uses .vert & .frag extensions");
		return;
	}
	else // Found
		vertexShaderPath_ = filePathVert;

	if (_autoLinkRHI && !fragmentShaderPath_.empty())
		GetRDR()->LoadShader(this);
}

void Resources::Shader::SetFragment(const string _name, bool _autoLinkRHI)
{
	fs::path filePathFrag = FindFile(_name + ".frag", type_);
	if (filePathFrag.empty()) // If no fragment shader found
	{
		DEBUG_LOG("Shader: FRAGMENT file not found for %s", _name.c_str());
		DEBUG_LOG("Shader class uses .vert & .frag extensions");
		return;
	}
	else // Found
		fragmentShaderPath_ = filePathFrag;

	if (_autoLinkRHI && !vertexShaderPath_.empty())
		GetRDR()->LoadShader(this);
}

void Resources::Shader::Use() const { GetRDR()->UseShader(this); }

void Resources::Shader::StopUse() const { GetRDR()->StopUseShader(); }

fs::path Resources::Shader::GetResourcePath() const
{
	DEBUG_WARNING("Shader Class has no single Path, use a more specific Getter");
	return fs::path();
}

fs::path Resources::Shader::GetVertexPath() const { return vertexShaderPath_; }

fs::path Resources::Shader::GetFragmentPath() const { return fragmentShaderPath_; }