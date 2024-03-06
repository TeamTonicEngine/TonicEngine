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

	u32 vertexShader = 0;
	u32 fragmentShader = 0;
	u32 shaderProgram = 0;

	u32 texture_id = 0;
	u32 shader_id = 0;

	u32 VBO, VAO, EBO, FBO, RBO;

	void Init(std::filesystem::path const _vertexPath, std::filesystem::path const _fragmentPath);
	std::string ReadFile(std::filesystem::path const _path);

private:
};

