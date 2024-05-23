#pragma once
#include "Core/RHI.hpp"

namespace Core::Renderer
{
	class OpenGLWrapper : public RHI
	{
		/*********************************************
				VARIABLES BLOC
		*********************************************/
	private:
		unsigned int skyboxVAO_ = -1;
		unsigned int skyboxVBO_ = -1;

		Resources::FontPtr p_currentFont_ = Resources::Font::s_p_defaultFont;
		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:

		const bool Init(Core::Applications::Window* _p_window) override;

		WindowBuffer* InitFrameBuffer() override;

		void StartFrame() override;
		void EndFrame() override;

		void BindFrameBuffer(WindowBuffer* _bufferObject) override;
		void ResizeFrameBufferImage(u32 _ID, u32 _width, u32 _height) override;
		void UnbindFrameBuffer() override;

		void Destroy() override;

		void ChangeClearColor(TNCColor _color) override;
		void ClearColor() override;

		void DepthMaskActive(bool _newValue) override;

		void ResizeViewPort(u32 width, u32 height) override;

		//---Synchronization-function-----------------------------------
		void DeviceWaitIdle() override;
		//---Callbacks--------------------------------------------------
		void FrameBufferResized() override;

		//---Low-Renderer-----------------------------------------------
		//------Matrices---(call to set uniform)------------------------
		//
		//-----------Model----------------------------------------------
		void SetModel(Maths::Mat4 _modelMatrix) override;

		//-----------Camera---------------------------------------------
		void SetCamera(const LowRenderer::Cameras::Camera* _p_camera, Maths::Vec3 _position) override;
		void SetFixedCamera(const LowRenderer::Cameras::Camera* _p_camera) override;
		void SetDefaultCamera() override;

		//-----------Light----------------------------------------------
		void SetDirectionalLightNumber(u32 _number) const override;
		void SetPointLightNumber(u32 _number) const override;
		void SetSpotLightNumber(u32 _number) const override;
		void SetLight(const LowRenderer::Lights::DirectionalLight* _p_light, u32 _index, Maths::Vec3 _direction) const override;
		void SetLight(const LowRenderer::Lights::PointLight* _p_light, u32 _index, Maths::Vec3 _position) const override;
		void SetLight(const LowRenderer::Lights::SpotLight* _p_light, u32 _index, Maths::Vec3 _position, Maths::Quat _rotation)const override;

		//---Resources---------------------------------------------------
		//
		//------Materials-----------------------------------------------
		void LoadResource(Resources::MaterialPtr _p_material) override;
		void UseResource(const Resources::MaterialPtr _p_material) override;
		void UseResource(const Resources::MaterialPtr _p_material, const int _shaderProgramIndex) override;
		void UnloadResource(const Resources::MaterialPtr _p_material) override;
		//
		//------Textures-----------------------------------------------
		void LoadResource(Resources::TexturePtr _p_texture) override;
		unsigned int LoadCubemap(std::vector<std::string> faces) override;
		void RenderCubeMap() override;
		//	 Takes the type inside the texture class
		void UseResource(const Resources::TexturePtr _p_texture) override;
		//   Overrides the type inside the texture class
		void UseResource(const Resources::TexturePtr _p_texture, Resources::Textures::TextureType _type);
		void StopUseTexture() override;
		void UnloadResource(const Resources::TexturePtr _p_texture) override;
		//
		//------Fonts--------------------------------------------------
		void LoadResource(Resources::FontPtr _p_font) override;
		void UseResource(const Resources::FontPtr _p_font) override;
		//   Avoid to send a Mat4 buffer twice and reset font Settings every call
		void UseResourceWithoutSafety(const Resources::FontPtr _p_font) override;
		void RenderText(std::string _text, float _x, float _y, Maths::Vec2 _scale, TNCColor _color = TNCColor{ 255,255,255,255 }) override;
		void TransformText(Maths::Mat4 _model) override;
		void DrawOnTop() override;
		void StopUseFonts() override;
		void UnloadResource(const Resources::FontPtr _p_font) override;
		//
		//------Shaders------------------------------------------------
		void LoadResource(Resources::ShaderPtr _p_shader) override;
		void UseResource(const Resources::ShaderPtr _p_shader) override;
		void StopUseShader() override;
		void UnloadResource(const Resources::ShaderPtr _p_shader) override;
		//
		//------Meshes-----------------------------------------------
		void LoadResource(Resources::MeshPtr _p_mesh) override;
		void UseResource(const Resources::MeshPtr _p_mesh) override;
		void UseResource(const Resources::MeshPtr _p_mesh, std::vector<Resources::MaterialPtr> p__materials) override;
		void UnloadResource(const Resources::MeshPtr _p_mesh) override;

	protected:
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
		void SetUniform(const int _shaderProgramIndex, const std::string& _name, TNCColor _value) const;
		void SetUniform(const std::string& _name, TNCColor _value) const;
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