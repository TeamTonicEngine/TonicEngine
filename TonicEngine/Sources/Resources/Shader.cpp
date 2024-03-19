#include "pch.hpp"
#include "TonicEngine/Resources/Shader.hpp"

#include "TonicEngine/Engine.hpp"


Resources::Shader::Shader(std::filesystem::path const _vertexPath, std::filesystem::path const _fragmentPath)
{
    vertexShaderPath_ = _vertexPath;
    vertexCode_ = ReadFile(vertexShaderPath_);

    fragmentShaderPath_ = _fragmentPath;
    fragmentCode_ = ReadFile(fragmentShaderPath_);
}

Resources::Shader::~Shader()
{

}

std::string Resources::Shader::ReadFile(std::filesystem::path const _path)
{
    std::ifstream shaderFile;
    std::string fileContent;
    // open files
    shaderFile.open(_path);
    try
    {
        if (shaderFile.bad())
        {
            DEBUG_ERROR("Shader file %s is BAD", _path.filename().c_str());
        }
        else if (shaderFile.fail())
        {
            DEBUG_WARNING("Shader file %s opening has FAILED", _path.filename().c_str());
        }
        else if (shaderFile.is_open())
        {
            fileContent = std::string((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
        } // Safe if file isn't open
    }
    catch (std::ifstream::failure& e)
    {
        DEBUG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: %s", e.what());
    }
    shaderFile.close();
    return fileContent;
}

void Resources::Shader::MetaWriteFile(const std::string _name)
{

}
void Resources::Shader::MetaReadFile(const std::string _name)
{

}

void Resources::Shader::ResourceUnload()
{

}

std::filesystem::path Resources::Shader::GetVertexPath() const
{
	return vertexShaderPath_;
}

std::filesystem::path Resources::Shader::GetFragmentPath() const
{
	return fragmentShaderPath_;
}


