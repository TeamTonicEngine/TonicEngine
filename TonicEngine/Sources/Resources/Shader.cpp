/*
#include "pch.hpp"
#include "Resources/Shader.hpp"
#include "Core/Log.hpp"

Shader::Shader()
{
}

void Shader::Init(std::filesystem::path const _vertexPath, std::filesystem::path const _fragmentPath)
{
    vertexCode_ = ReadFile(_vertexPath);
    fragmentCode_ = ReadFile(_fragmentPath);
}

std::string Shader::ReadFile(std::filesystem::path const _path)
{   

    std::istream shaderFile;
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
    catch (std::istd::filesystemtream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    shaderFile.close();
    return fileContent;
}
*/

#include "pch.hpp"
#include "TonicEngine/Resources/Shader.hpp"

#include "TonicEngine/Engine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../Externals/include/STB/stb_image.hpp"

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

void Resources::Shader::FlipVertically(bool _isOn)
{
    stbi_set_flip_vertically_on_load(_isOn);
}

void* Resources::Shader::LoadTexture(std::string _path, int* width, int* height, int* nrChannels)
{
    return stbi_load(_path.c_str(), width, height, nrChannels, 0);
}

void Resources::Shader::FreeImage(void* _data)
{
    stbi_image_free(_data);
}

