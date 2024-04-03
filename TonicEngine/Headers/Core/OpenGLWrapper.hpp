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
		const bool Init(Core::Applications::Window* _p_window) override;

		void InitFrameBuffer() override;

		void StartFrame() override;
		void Draw() override;
		void EndFrame() override;

		void BindFrameBuffer() override;
		void UnbindFrameBuffer() override;

		void Destroy() override;

		void ChangeClearColor(Color _color) override;
		void ClearColor() override;

		void ResizeViewPort(u32 width, u32 height) override;

		//---Synchronization-function-----------------------------------
		void DeviceWaitIdle() override;
		//---Callbacks--------------------------------------------------
		void FrameBufferResized() override;

		//---Low-Renderer-----------------------------------------------
		//------Matrices---(call to set uniform)------------------------
		//-----------Model----------------------------------------------
		void SetModel(Maths::Mat4 _modelMatrix) override;
		//-----------Camera---------------------------------------------
		//void SetCamera(Camera* _p_newCamera) override; // Implentation in RHI class
		void UpdateCurrentCamera(const LowRenderer::Cameras::CameraInput* _camInputs) override;
		//-----------Light----------------------------------------------
		void SetLight(LowRenderer::ILight* _p_newLight) override;
		void SetLights(std::vector<LowRenderer::ILight*> _lightList) override;

		//---Resources---------------------------------------------------
		//void LoadResource(Resources::IResource* _p_resource) override; // Implementation in RHI class
		//void UseResource(const Resources::IResource* _p_resource) override; // Implementation in RHI class
		//void UnloadResource(const Resources::IResource* _p_resource) override; // Implementation in RHI class
		//------Materials-----------------------------------------------
		void LoadMaterial(Resources::Material* _p_material) override;
		void UseMaterial(const Resources::Material* _p_material) override;
		void UnloadMaterial(const Resources::Material* _p_material) override;
		//------Textures-----------------------------------------------
		void LoadTexture(Resources::Texture* _p_texture) override;
		void UseTexture(const Resources::Texture* _p_texture) override;
		void StopUseTexture();
		void UnloadTexture(const Resources::Texture* _p_texture) override;
		//------Shader-------------------------------------------------
		void LoadShader(Resources::Shader* _p_shader) override;
		void UseShader(const Resources::Shader* _p_shader) override;
		void StopUseShader() override;
		void AddPostprocessShader(const Resources::Shader* _p_shader) override;
		void UnloadShader(const Resources::Shader* _p_shader) override;
		//------Mesh-----------------------------------------------
		void LoadMesh(Resources::BasicMesh* _p_mesh) override;
		void UseMesh(const Resources::Mesh* _p_mesh) override;
		void UnloadMesh(const Resources::Mesh* _p_mesh) override;

		//------Uniforms-------------------------------------------
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, bool _value) const override;
		void SetUniform(const std::string& _name, bool _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, int _value) const override;
		void SetUniform(const std::string& _name, int _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, unsigned _value) const override;
		void SetUniform(const std::string& _name, unsigned _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _value) const override;
		void SetUniform(const std::string& _name, float _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec2 _value) const override;
		void SetUniform(const std::string& _name, Maths::Vec2 _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY) const override;
		void SetUniform(const std::string& _name, float _valueX, float _valueY) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec3 _value) const override;
		void SetUniform(const std::string& _name, Maths::Vec3 _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY, float _valueZ) const override;
		void SetUniform(const std::string& _name, float _valueX, float _valueY, float _valueZ) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec4 _value) const override;
		void SetUniform(const std::string& _name, Maths::Vec4 _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, Color _value) const;
		void SetUniform(const std::string& _name, Color _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY, float _valueZ, float _valueW) const override;
		void SetUniform(const std::string& _name, float _valueX, float _valueY, float _valueZ, float _valueW) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Mat3 _value) const override;
		void SetUniform(const std::string& _name, Maths::Mat3 _value) const;
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Mat4 _value) const override;

		void SetUniform(const std::string& _name, Maths::Mat4 _value) const;

	private:
		/* Checks if shader compilation failled and prints error in log
		Output : Returns true on error detected */
		bool CheckCompileErrors(unsigned& _shaderID, std::string _type);
	};
}