#pragma once
#include <iostream>
#include "TonicEngine\DLL_API.hpp"
#include "TonicEngine\Resources\Shader.hpp"

namespace Core
{
	class TONIC_ENGINE_API RHI
	{
	public:

		virtual void Init(uint32_t width, uint32_t height);
		virtual void StartFrame();

		// Shader
		virtual void InitShader();
		virtual void DrawTriangle();

		
		virtual void InitFrameBuffer();
		virtual void BindFrameBuffer();
		virtual void UnbindFrameBuffer();
		virtual void RescaleFrameBuffer(float width, float height);
		virtual void ResetViewPort(float width, float height);

		virtual unsigned int GetTextureID();
		virtual void CleanUp();
	};
}