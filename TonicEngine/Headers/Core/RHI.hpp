#pragma once
#include "DLL_API.hpp"

#include <string>
#include <vector>

// Utils and Math
#include <Core/Utils/TNCColor.hpp>
#include "Maths/Maths.hpp"

// Core
#include "Core/Window.hpp"

// Cameras
#include "LowRenderer/Cameras/Camera.hpp"
#include "LowRenderer/Cameras/FreeCamera.hpp"

// Lights
#include "LowRenderer/Lights/Lights.hpp"

// Resources
#include "Resources/Resources.hpp"

struct TONIC_ENGINE_API WindowBuffer
{
	unsigned int frame = -1;
	unsigned int ID = -1;
	unsigned int render = -1;
};

namespace Core::Renderer
{
	class RHI
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		Core::Applications::Window* p_window_ = nullptr;
		LowRenderer::Cameras::FreeCamera* p_currentCamera_ = nullptr;
		TNCColor clearColor_ = { 165, 130, 173, 255 };
		unsigned shaderInUse_ = -1;
		unsigned int cubeMap_ = -1;
		std::vector<WindowBuffer*> p_bufferObject_;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		virtual const bool TONIC_ENGINE_API Init(Core::Applications::Window* _p_window) { p_window_ = _p_window; return p_window_; };
		virtual WindowBuffer* InitFrameBuffer() = 0;

		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void BindFrameBuffer(WindowBuffer* _bufferObject) = 0;
		virtual void ResizeFrameBufferImage(u32 _ID, u32 _width, u32 _height) = 0;

		virtual void UnbindFrameBuffer() = 0;

		virtual void Destroy() = 0;

		virtual void ChangeClearColor(TNCColor _color) = 0;
		TNCColor TONIC_ENGINE_API GetClearColor() { return clearColor_; }
		virtual void TONIC_ENGINE_API ClearColor() = 0;

		virtual void DepthMaskActive(bool _newValue) = 0;

		virtual void ResizeViewPort(u32 _width, u32 _height) = 0;

		//---Synchronization-function-----------------------------------
		virtual void DeviceWaitIdle() = 0;
		//---Callbacks--------------------------------------------------
		virtual void FrameBufferResized() = 0;

		//---Low-Renderer-----------------------------------------------
		//------Matrices---(call to set uniform)------------------------
		//-----------Model----------------------------------------------
		virtual void SetModel(Maths::Mat4 _modelMatrix) = 0;
		//-----------Camera---------------------------------------------
		virtual void SetDefaultCamera() = 0;
		virtual void SetCamera(LowRenderer::Cameras::FreeCamera* _p_newCamera) { p_currentCamera_ = _p_newCamera; };
		virtual void SetFixedCamera(const LowRenderer::Cameras::Camera* _p_camera) = 0;
		const LowRenderer::Cameras::FreeCamera* GetCamera() const { return p_currentCamera_; };
		virtual void SetCamera(const LowRenderer::Cameras::Camera* _p_camera, Maths::Vec3 _position) = 0;
		//-----------Light----------------------------------------------
		virtual void SetDirectionalLightNumber(u32 _number) const = 0;
		virtual void SetPointLightNumber(u32 _number) const = 0;
		virtual void SetSpotLightNumber(u32 _number) const = 0;
		virtual void SetLight(const LowRenderer::Lights::DirectionalLight* _p_light, u32 _index, Maths::Vec3 _direction) const = 0;
		virtual void SetLight(const LowRenderer::Lights::PointLight* _p_light, u32 _index, Maths::Vec3 _position) const = 0;
		virtual void SetLight(const LowRenderer::Lights::SpotLight* _p_light, u32 _index, Maths::Vec3 _position, Maths::Quat _rotation) const = 0;

		//---Resources---------------------------------------------------
		//------Materials-----------------------------------------------
		virtual void LoadResource(Resources::MaterialPtr _p_material) = 0;
		virtual void UseResource(const Resources::MaterialPtr _p_material) = 0;
		virtual void UseResource(const Resources::MaterialPtr _p_material, const int _shaderProgramIndex) = 0;
		virtual void UnloadResource(const Resources::MaterialPtr _p_material) = 0;

		//------Textures-----------------------------------------------
		virtual void LoadResource(Resources::TexturePtr _p_texture) = 0;
		virtual unsigned int LoadCubemap(std::vector<std::string> faces) = 0;
		virtual void RenderCubeMap() = 0;
		//----//Takes the type inside the texture class
		virtual void UseResource(const Resources::TexturePtr _p_texture) = 0;
		//----//Overrides the type inside the texture class
		virtual void UseResource(const Resources::TexturePtr _p_texture, Resources::Textures::TextureType _type) = 0;
		virtual void StopUseTexture() = 0;
		virtual void UnloadResource(const Resources::TexturePtr _p_texture) = 0;

		//------Fonts--------------------------------------------------
		virtual void LoadResource(Resources::FontPtr _p_font) = 0;
		virtual void UseResource(const Resources::FontPtr _p_font) = 0;
		// Avoid to send a Mat4 buffer twice and reset font Settings every call
		virtual void UseResourceWithoutSafety(const Resources::FontPtr _p_font) = 0;
		virtual void TransformText(Maths::Mat4 _model) = 0;
		// Think about using the font before
		virtual void RenderText(std::string _text, float _x, float _y, Maths::Vec2 _scale, TNCColor _color = { 255,255,255,255 }) = 0;
		virtual void DrawOnTop() = 0;
		virtual void StopUseFonts() = 0;
		virtual void UnloadResource(const Resources::FontPtr _p_font) = 0;

		//------Shader-------------------------------------------------
		virtual void LoadResource(Resources::ShaderPtr _p_shader) = 0;
		virtual void UseResource(const Resources::ShaderPtr _p_shader) = 0;
		virtual void StopUseShader() = 0;
		virtual void UnloadResource(const Resources::ShaderPtr _p_shader) = 0;
		unsigned GetShaderInUse() const { return shaderInUse_; };

		//------Mesh-----------------------------------------------
		virtual void LoadResource(Resources::MeshPtr _p_mesh) = 0;
		virtual void UseResource(const Resources::MeshPtr _p_mesh) = 0;
		virtual void UseResource(const Resources::MeshPtr _p_mesh, std::vector<Resources::MaterialPtr> p__materials) = 0;
		virtual void UnloadResource(const Resources::MeshPtr _p_mesh) = 0;

	protected:
		//------Uniforms-------------------------------------------
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, bool _value) const = 0;
		virtual void SetUniform(const std::string& _name, bool _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, int _value) const = 0;
		virtual void SetUniform(const std::string& _name, int _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, unsigned int _value) const = 0;
		virtual void SetUniform(const std::string& _name, unsigned int _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _value) const = 0;
		virtual void SetUniform(const std::string& _name, float _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec2 _value) const = 0;
		virtual void SetUniform(const std::string& _name, Maths::Vec2 _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY) const = 0;
		virtual void SetUniform(const std::string& _name, float _valueX, float _valueY) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec3 _value) const = 0;
		virtual void SetUniform(const std::string& _name, Maths::Vec3 _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY, float _valueZ) const = 0;
		virtual void SetUniform(const std::string& _name, float _valueX, float _valueY, float _valueZ) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec4 _value) const = 0;
		virtual void SetUniform(const std::string& _name, Maths::Vec4 _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, TNCColor _value) const = 0;
		virtual void SetUniform(const std::string& _name, TNCColor _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY, float _valueZ, float _valueW) const = 0;
		virtual void SetUniform(const std::string& _name, float _valueX, float _valueY, float _valueZ, float _valueW) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Mat3 _value) const = 0;
		virtual void SetUniform(const std::string& _name, Maths::Mat3 _value) const = 0;
		virtual void SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Mat4 _value) const = 0;
		virtual void SetUniform(const std::string& _name, Maths::Mat4 _value) const = 0;
	};
}