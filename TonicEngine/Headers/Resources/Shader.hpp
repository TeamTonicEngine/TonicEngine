#pragma once

#include "DLL_API.hpp"

#include "IResource.hpp"

namespace Resources
{
	class Shader : public IResource
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		fs::path vertexShaderPath_ = fs::path();
		fs::path fragmentShaderPath_ = fs::path();

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		TONIC_ENGINE_API Shader();
		TONIC_ENGINE_API ~Shader();

		void TONIC_ENGINE_API ReadFile(const string _name) override;
		void TONIC_ENGINE_API ReadFile(const fs::path _path) override;

		void TONIC_ENGINE_API MetaWriteFile(const string _name) override {};
		void TONIC_ENGINE_API MetaReadFile(const string _name) override {};

		void TONIC_ENGINE_API ResourceUnload() override {};

		/* Input : Only the name of the shader is needed (not the location) */
		void TONIC_ENGINE_API SetVertex(const string _name, bool _autoLinkRHI = true);
		/* Input : Only the name of the shader is needed (not the location) */
		void TONIC_ENGINE_API SetFragment(const string _name, bool _autoLinkRHI = true);

		void TONIC_ENGINE_API Use() const override;
		void TONIC_ENGINE_API StopUse() const;

		fs::path GetResourcePath() const override;

		fs::path TONIC_ENGINE_API GetVertexPath() const;
		fs::path TONIC_ENGINE_API GetFragmentPath() const;
	};
}