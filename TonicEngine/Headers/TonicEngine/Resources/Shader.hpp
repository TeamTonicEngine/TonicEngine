
#pragma once
#include <TonicEngine/DLL_API.hpp>
#include "IResource.hpp"

namespace Resources
{
	class TONIC_ENGINE_API Shader : public IResource
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		std::filesystem::path vertexShaderPath_ = std::filesystem::path();
		std::filesystem::path fragmentShaderPath_ = std::filesystem::path();
	public:

		std::string vertexCode_;
		std::string fragmentCode_;

		u32 vertexShader = 0;
		u32 fragmentShader = 0;
		u32 shaderProgram = 0;

		u32 texture_id = 0;
		u32 shader_id = 0;

		u32 VBO, VAO, EBO, FBO, RBO;

		unsigned int texture1, texture2;
		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
		Shader(std::filesystem::path const _vertexPath, std::filesystem::path const _fragmentPath);
		~Shader();

		std::string ReadFile(std::filesystem::path const _path) override;
		void MetaWriteFile(const string _name) override;
		void MetaReadFile(const string _name) override;

		void ResourceUnload() override;


		std::filesystem::path GetVertexPath() const;
		std::filesystem::path GetFragmentPath() const;

		void FlipVertically(bool _isOn);
		void* LoadTexture(std::string _path, int* width, int* height, int* nrChannels);
		void FreeImage(void* _data);
	};
}


