#pragma once
#include <iostream>

#include "TonicEngine\DLL_API.hpp"

#include "TonicEngine\Resources\IResource.hpp"

namespace Core
{
	class TONIC_ENGINE_API RHI
	{
	public:

		virtual void Init(u32 width, u32 height);
		virtual void StartFrame();

		// Shader
		virtual void InitShader();
		virtual void CheckCompileErrors(unsigned int shader, std::string type);
		virtual void InitShaderData();
		virtual void DrawTriangle();

		void LoadResource(const Resources::IResource* _p_resource);
		void UnloadResource(const Resources::IResource* _p_resource);
		
		virtual void InitFrameBuffer();
		virtual void BindFrameBuffer();
		virtual void UnbindFrameBuffer();
		virtual void RescaleFrameBuffer(f32 width, f32 height);
		virtual void ResetViewPort(f32 width, f32 height);

		virtual u32 GetTextureID();
		virtual void CleanUp();
	};
}