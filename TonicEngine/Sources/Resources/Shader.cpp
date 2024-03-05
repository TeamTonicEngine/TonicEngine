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
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    shaderFile.close();
    return fileContent;
}