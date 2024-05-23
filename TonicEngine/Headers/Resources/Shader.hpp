#pragma once
#include "IResource.hpp"

namespace Core::Renderer { class OpenGLWrapper; }

namespace Resources
{
	class Shader : public IResource, public std::enable_shared_from_this<Shader>
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		fs::path vertexShaderPath_ = fs::path();
		fs::path fragmentShaderPath_ = fs::path();
		std::string vertexCode_ = "";
		std::string fragmentCode_ = "";

		friend class Core::Renderer::OpenGLWrapper;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		TONIC_ENGINE_API Shader();
		TONIC_ENGINE_API ~Shader() = default;

		void TONIC_ENGINE_API Destroy() override;

		void TONIC_ENGINE_API ReadFile(const fs::path _path) override;
		void TONIC_ENGINE_API LoadFile();

		void TONIC_ENGINE_API ResourceUnload() override {};

		/* Input : Only the name of the shader is needed (not the location) */
		void TONIC_ENGINE_API SetVertex(const std::string _name);
		/* Input : Only the name of the shader is needed (not the location) */
		void TONIC_ENGINE_API SetFragment(const std::string _name);

		void TONIC_ENGINE_API Use() override;
		void TONIC_ENGINE_API StopUse() const;

		fs::path GetResourcePath() const override;

		fs::path TONIC_ENGINE_API GetVertexPath() const;
		fs::path TONIC_ENGINE_API GetFragmentPath() const;
	};
	using ShaderPtr = std::shared_ptr<Shader>;
}