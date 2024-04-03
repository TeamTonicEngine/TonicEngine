#pragma once

#include <string>
#include <vector>

//Utils
#include <Core/Utils/Color.hpp>
#include "Maths/Maths.hpp"

//Core
#include "Core/Window.hpp"

//LowRenderer
namespace LowRenderer::Cameras
{
	class Camera;
}
namespace LowRenderer
{
	class ILight;
}

//Resources
#include "Resources/IResource.hpp"
#include "Resources/Shader.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
namespace Resources
{
	class Material;
}

namespace Core::Renderer
{
	class TONIC_ENGINE_API RHI
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		Core::Applications::Window* p_window_ = nullptr;
		LowRenderer::Cameras::FreeCamera* p_currentCamera_ = nullptr;
		Color clearColor_ = { 165, 130, 173, 255 };
		unsigned shaderInUse_ = -1, frameBufferObject = -1, frameBufferID = -1, renderBufferObject = -1;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		virtual const bool Init(Core::Applications::Window* _p_window);
		virtual void InitFrameBuffer() = 0;

		virtual void StartFrame() = 0;
		virtual void Draw() = 0;
		virtual void EndFrame() = 0;

		virtual void BindFrameBuffer() = 0;
		virtual void UnbindFrameBuffer() = 0;
		const unsigned GetFrameBufferID() const;

		virtual void Destroy() = 0;

		virtual void ChangeClearColor(Color _color) = 0;
		Color GetClearColor() { return clearColor_; }
		virtual void ClearColor() = 0;

		virtual void ResizeViewPort(u32 width, u32 height) = 0;

		//---Synchronization-function-----------------------------------
		virtual void DeviceWaitIdle() = 0;
		//---Callbacks--------------------------------------------------
		virtual void FrameBufferResized() = 0;

		//---Low-Renderer-----------------------------------------------
		//------Matrices---(call to set uniform)------------------------
		//-----------Model----------------------------------------------
		virtual void SetModel(Maths::Mat4 _modelMatrix) = 0;
		//-----------Camera---------------------------------------------
		virtual void SetCamera(LowRenderer::Cameras::FreeCamera* _p_newCamera);
		virtual void UpdateCurrentCamera(const LowRenderer::Cameras::CameraInput* _camInputs) = 0;
		//-----------Light----------------------------------------------
		virtual void SetLight(LowRenderer::ILight* _p_newLight) = 0;
		virtual void SetLights(std::vector<LowRenderer::ILight*> _lightList) = 0;

		//---Resources---------------------------------------------------
		virtual void LoadResource(Resources::IResource* _p_resource);
		virtual void UseResource(const Resources::IResource* _p_resource);
		virtual void UnloadResource(const Resources::IResource* _p_resource);
		//------Materials-----------------------------------------------
		virtual void LoadMaterial(Resources::Material* _p_material) = 0;
		virtual void UseMaterial(const Resources::Material* _p_material) = 0;
		virtual void UnloadMaterial(const Resources::Material* _p_material) = 0;
		//------Textures-----------------------------------------------
		virtual void LoadTexture(Resources::Texture* _p_texture) = 0;
		virtual void UseTexture(const Resources::Texture* _p_texture) = 0;
		virtual void StopUseTexture() = 0;
		virtual void UnloadTexture(const Resources::Texture* _p_texture) = 0;
		//------Shader-------------------------------------------------
		virtual void LoadShader(Resources::Shader* _p_shader) = 0;
		virtual void UseShader(const Resources::Shader* _p_shader) = 0;
		virtual void StopUseShader() = 0;
		virtual void AddPostprocessShader(const Resources::Shader* _p_shader) = 0;
		virtual void UnloadShader(const Resources::Shader* _p_shader) = 0;
		unsigned GetShaderInUse()const { return shaderInUse_; };
		//------Mesh-----------------------------------------------
		virtual void LoadMesh(Resources::BasicMesh* _p_mesh) = 0;
		virtual void UseMesh(const Resources::Mesh* _p_mesh) = 0;
		virtual void UnloadMesh(const Resources::Mesh* _p_mesh) = 0;

		//------Uniforms-------------------------------------------
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, bool _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, int _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, unsigned int _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec2 _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec3 _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY, float _valueZ) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec4 _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY, float _valueZ, float _valueW) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Mat3 _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Mat4 _value) const = 0;

		virtual void SetUniform(const std::string& _name, bool _value) const = 0;
		virtual void SetUniform(const std::string& _name, int _value) const = 0;
		virtual void SetUniform(const std::string& _name, unsigned int _value) const = 0;
		virtual void SetUniform(const std::string& _name, float _value) const = 0;
		virtual void SetUniform(const std::string& _name, Maths::Vec2 _value) const = 0;
		virtual void SetUniform(const std::string& _name, float _valueX, float _valueY) const = 0;
		virtual void SetUniform(const std::string& _name, Maths::Vec3 _value) const = 0;
		virtual void SetUniform(const std::string& _name, float _valueX, float _valueY, float _valueZ) const = 0;
		virtual void SetUniform(const std::string& _name, Maths::Vec4 _value) const = 0;
		virtual void SetUniform(const std::string& _name, float _valueX, float _valueY, float _valueZ, float _valueW) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, Color _value) const = 0;
		virtual void SetUniform(const std::string& _name, Color _value) const = 0;
		virtual void SetUniform(const std::string& _name, Maths::Mat3 _value) const = 0;
		virtual void SetUniform(const std::string& _name, Maths::Mat4 _value) const = 0;
	};
}