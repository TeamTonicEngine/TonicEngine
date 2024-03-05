#pragma once
#include <TonicEngine/DLL_API.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum class TONIC_ENGINE_API ShaderType
{
	TYPE_ERROR,
	FRAGMENT = 0x8B30,
	VERTEX = 0x8B31
};

class TONIC_ENGINE_API Shader
{
public:
	Shader();
	std::string vertexCode_;
	std::string fragmentCode_;

	unsigned int vertexShader = 0;
	unsigned int fragmentShader = 0;
	unsigned int shaderProgram = 0;

	unsigned int texture_id = 0;
	unsigned int shader_id = 0;

	unsigned int VBO, VAO, EBO, FBO, RBO;

	void Init(std::filesystem::path const _vertexPath, std::filesystem::path const _fragmentPath);
	std::string ReadFile(std::filesystem::path const _path);

private:
};

