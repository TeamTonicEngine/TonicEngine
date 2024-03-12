#pragma once
#include <iostream>
#include <vector>
#include <map>

#include "TonicEngine\DLL_API.hpp"

#include <TonicEngine/Resources/Shader.hpp>
#include <TonicEngine/Resources/Texture.hpp>
#include "Maths.hpp"

namespace Core
{
	class TONIC_ENGINE_API RHI
	{
	public:
		/*********************************************
				VARIABLES BLOC
		*********************************************/
		std::map < std::string, Resources::Shader* > shader_;
		std::vector<Resources::Texture*> texture_;
		//std::map < std::string, Resources::Texture* > texture_;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
		virtual void Init(u32 width, u32 height);
		virtual void StartFrame();
		virtual void Draw();
		virtual void EndFrame();

		//---Callbacks--------------------------------------------------
		//virtual void FrameBufferResized(s32 width, s32 height);
		virtual void RescaleFrameBuffer(s32 width, s32 height);
		virtual u32 GetTextureID();
		virtual void InitFrameBuffer();
		virtual void Transform();
		//---Low-Renderer-----------------------------------------------
		// 
		//------Model----------------------------------------------
		//------Camera---------------------------------------------
		//------Light----------------------------------------------


		//---Resources---------------------------------------------------
		// 
		//------Shader---------------------------------------------
		virtual void InitShader();
		virtual void InitShaderData();
		virtual void CheckShaderCompileErrors(unsigned int shader, std::string type);
		virtual void BindFrameBuffer();
		virtual void UnbindFrameBuffer();

		virtual void SetInt(const std::string& _shaderName, const std::string& _name, int _value);
		//------Textures-------------------------------------------
		//virtual void InitTexture();
		//virtual unsigned int LoadTexture(std::string _name);
		//------Materials------------------------------------------
		//------Mesh-----------------------------------------------

		virtual void CleanUp();
	};
}