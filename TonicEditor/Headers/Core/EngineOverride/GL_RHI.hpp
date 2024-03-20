#pragma once
#include "TonicEngine/Core/RHI.hpp"
#include <TonicEngine/Resources/Shader.hpp>
#include "Core/EngineOverride/GL_Model.hpp"
#include <TonicEngine/Resources/Sound.hpp>

using namespace Resources;


class GL_RHI : public Core::RHI
{
public:
	/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	GL_RHI();
	~GL_RHI();
	Sound* soundClap;
	std::vector<GL_Model*> model_;

	void Init(uint32_t width, uint32_t height) override;
	void StartFrame(FreeCamera* _camera) override;
	void Draw() override;
	void EndFrame() override;

	//---Callbacks--------------------------------------------------
	//void FrameBufferResized(s32 width, s32 height);
	void Transform(FreeCamera* _camera) override;
	//---Low-Renderer-----------------------------------------------
	// 
	//------Model----------------------------------------------
	//------Camera---------------------------------------------
	//------Light----------------------------------------------


	//---Resources---------------------------------------------------
	// 
	//------Shader---------------------------------------------
	void InitShader() override;
	void CheckShaderCompileErrors(unsigned int shader, std::string type) override;
	void BindFrameBuffer() override;
	void UnbindFrameBuffer() override;
	void InitFrameBuffer() override;

	//unsigned int GetTextureID(std::string _name) override;
	void ShaderUse(std::string _shaderName);

	// utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& _shaderName, const std::string& _name, int _value) override;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, const Maths::Vec2& value) const;
	void SetVec2(const std::string& name, float x, float y) const;
	void SetVec3(const std::string& name, const Maths::Vec3& value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;
	void SetVec4(const std::string& name, const Maths::Vec4& value) const;
	void SetVec4(const std::string& name, float x, float y, float z, float w);
	void SetMat3(const std::string& name, const Maths::Mat3& mat) const;
	void SetMat4(const std::string& _shaderName, const std::string& name, const Maths::Mat4& mat);
	
	//------Textures-------------------------------------------
	//void InitTexture() override;
	//unsigned int LoadTexture(std::string _name) override;
	//------Materials------------------------------------------
	//------Model----------------------------------------------

	void CleanUp() override;
	u32 GetTextureID() override;
	void RescaleFrameBuffer(s32 width, s32 height) override;

private:
	uint32_t width_, height_;
};