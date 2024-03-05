#pragma once

#include "Core/RHI.hpp"

namespace Core::Renderer
{
	class TONIC_ENGINE_API OpenGLWrapper : public RHI
	{
		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		void Init(Core::Applications::Window* _p_window);
		void Draw();
		void Destroy();

		void ChangeClearColor(Color _color);

		//---Synchronization-function-----------------------------------
		void DeviceWaitIdle();
		//---Callbacks--------------------------------------------------
		void FrameBufferResized();

		//---Low-Renderer-----------------------------------------------
		//------Matrices---(call to set uniform)------------------------
		//-----------Model----------------------------------------------
		void SetModel(Maths::Mat4 _modelMatrix);
		//-----------Camera---------------------------------------------
		//void SetCamera(Camera* _p_newCamera); // Implentation in RHI class
		//-----------Light----------------------------------------------
		void SetLight(LowRenderer::ILight* _p_newLight);
		void SetLights(std::vector<LowRenderer::ILight*> _lightList);

		//---Resources---------------------------------------------------
		//void LoadResource(const Resources::IResource* _p_resource); // Implementation in RHI class
		//void UseResource(const Resources::IResource* _p_resource); // Implementation in RHI class
		//void UnloadResource(const Resources::IResource* _p_resource); // Implementation in RHI class
		//------Materials-----------------------------------------------
		void LoadMaterial(const Resources::Material* _p_material);
		void UseMaterial(const Resources::Material* _p_material);
		void UnloadMaterial(const Resources::Material* _p_material);
		//------Textures-----------------------------------------------
		void LoadTexture(const Resources::Texture* _p_texture);
		void UseTexture(const Resources::Texture* _p_texture);
		void UnloadTexture(const Resources::Texture* _p_texture);
		//------Shader-------------------------------------------------
		void LoadShader(const Resources::Shader* _p_shader);
		void UseShader(const Resources::Shader* _p_shader);
		void StopUseShader();
		void AddPostprocessShader(const Resources::Shader* _p_shader);
		void UnloadShader(const Resources::Shader* _p_shader);
		//------Mesh-----------------------------------------------
		void LoadMesh(const Resources::Mesh* _p_mesh);
		void UseMesh(const Resources::Mesh* _p_mesh);
		void UnloadMesh(const Resources::Mesh* _p_mesh);

	protected:
		//------Uniforms-------------------------------------------
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, bool _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, int _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, unsigned int _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec2 _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec3 _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY, float _valueZ) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec4 _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY, float _valueZ, float _valueW) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Mat3 _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Mat4 _value) const;

	private:
	};
}