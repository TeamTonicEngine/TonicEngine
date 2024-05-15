#include "pch.hpp"

#include "Core/OpenGLWrapper.hpp"

#include <glad/glad.hpp>

#include "Core/Log.hpp"

#include "Resources/Resources.hpp"

//LowRenderer
#include"LowRenderer/Lights/Lights.hpp"

#pragma region DISPLAY
const bool Core::Renderer::OpenGLWrapper::Init(Core::Applications::Window* _p_window)
{
	RHI::Init(_p_window);

	if (!_p_window)
		throw ("Nullptr window sent to Core::Renderer::OpenGLWrapper::Init()");

	if (!gladLoadGLLoader((GLADloadproc)_p_window->GetProcAddress()))
		throw ("FAILED TO INITIALIZE GLAD");
	else
		DEBUG_SUCCESS("INITIALIZED GLAD");

	unsigned* size = p_window_->GetScreenSize();
	glViewport(0, 0, size[0], size[1]);
	f32_4 clear = clearColor_.ToFloat();
	glClearColor(clear.x, clear.y, clear.z, clear.w);
	//Anti-aliasing
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true; // if this is reached Init succeeded
}

WindowBuffer* Core::Renderer::OpenGLWrapper::InitFrameBuffer()
{
	//Create Frame Buffer Object
	WindowBuffer* newBufferObject = new WindowBuffer();

	glGenFramebuffers(1, &newBufferObject->frame);
	glBindFramebuffer(GL_FRAMEBUFFER, newBufferObject->frame);

	glGenTextures(1, &newBufferObject->ID);
	glBindTexture(GL_TEXTURE_2D, newBufferObject->ID);

	auto size = ENGINE.WDW->GetScreenSize();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size[0], size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, newBufferObject->ID, 0);

	//Create Render Buffer Object
	glGenRenderbuffers(1, &newBufferObject->render);
	glBindRenderbuffer(GL_RENDERBUFFER, newBufferObject->render);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size[0], size[1]);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, newBufferObject->render);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw ("FAILED TO INITIALIZE FRAMEBUFFER");
	else
		DEBUG_SUCCESS("INITIALIZED FRAMEBUFFER");

	//Unbind buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	p_bufferObject_.push_back(newBufferObject);
	return newBufferObject;
}

void Core::Renderer::OpenGLWrapper::StartFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void Core::Renderer::OpenGLWrapper::EndFrame() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Core::Renderer::OpenGLWrapper::BindFrameBuffer(WindowBuffer* _bufferObject)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _bufferObject->frame);
	glBindRenderbuffer(GL_RENDERBUFFER, _bufferObject->render);
}

void Core::Renderer::OpenGLWrapper::ResizeFrameBufferImage(u32 _ID, u32 _width, u32 _height)
{
	glBindTexture(GL_TEXTURE_2D, _ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
}

void Core::Renderer::OpenGLWrapper::UnbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Core::Renderer::OpenGLWrapper::Destroy()
{
	glDisable(GL_DEPTH_TEST);
	for (int i = 0; i < p_bufferObject_.size(); i++)
	{
		glDeleteBuffers(1, &p_bufferObject_[i]->frame);
		glDeleteBuffers(1, &p_bufferObject_[i]->render);
	}
}

void Core::Renderer::OpenGLWrapper::ChangeClearColor(TNCColor _color)
{
	clearColor_ = _color;
	f32_4 clear = _color.ToFloat();
	glClearColor(clear.x, clear.y, clear.z, clear.w);
}

void Core::Renderer::OpenGLWrapper::ClearColor() { glClear(GL_COLOR_BUFFER_BIT); }

void Core::Renderer::OpenGLWrapper::DepthMaskActive(bool _newValue)
{
	glDepthMask(_newValue);
}

void Core::Renderer::OpenGLWrapper::ResizeViewPort(u32 width, u32 height) { glViewport(0, 0, width, height); }

void Core::Renderer::OpenGLWrapper::DeviceWaitIdle() { /*EMPTY*/ }

void Core::Renderer::OpenGLWrapper::FrameBufferResized()
{
	unsigned* size = p_window_->GetScreenSize();
	ResizeViewPort(size[0], size[1]);
}

#pragma endregion

/*
	LOW RENDERER FUNCTIONS
*/
#pragma region LOW_RENDERER
void Core::Renderer::OpenGLWrapper::SetModel(Maths::Mat4 _modelMatrix)
{
	SetUniform("model", _modelMatrix);
}

void Core::Renderer::OpenGLWrapper::SetCamera(const LowRenderer::Cameras::Camera* _p_camera, Maths::Vec3 _position)
{
	SetUniform("viewPos", _position);
	SetUniform("viewProjection", _p_camera->viewProjection);
}
void Core::Renderer::OpenGLWrapper::SetFixedCamera(const LowRenderer::Cameras::Camera* _p_camera)
{
	Maths::Mat3 fixedView = _p_camera->view;
	Maths::Mat4 fixedView44 = fixedView;
	Maths::Mat4 nProj;
	if (_p_camera->bPerspectiveMode == false)
	{
		for (int i = 0; i < 4; i++)
			nProj.data4V[i] = _p_camera->projection.data4V[i].GetNormalized();
	}
	else
	{
		nProj = _p_camera->projection;
	}
	SetUniform("viewProjection", nProj * fixedView);
}
void Core::Renderer::OpenGLWrapper::SetDefaultCamera()
{
	SetCamera(p_currentCamera_, p_currentCamera_->eye);
}

void Core::Renderer::OpenGLWrapper::UpdateCurrentCamera(const LowRenderer::Cameras::CameraInput* _camInputs)
{
	if (!p_currentCamera_)
	{
		DEBUG_ERROR("Core::Renderer::OpenGLWrapper::UpdateCurrentCamera called but no current camera is set");
		return;
	}

	p_currentCamera_->ProcessInput(HEART::GetDeltaTime(), *_camInputs);
	p_currentCamera_->Update();
}

void Core::Renderer::OpenGLWrapper::SetDirectionalLightNumber(u32 _number) const { SetUniform("nbDirLights", _number); }

void Core::Renderer::OpenGLWrapper::SetPointLightNumber(u32 _number) const { SetUniform("nbPointLights", _number); }

void Core::Renderer::OpenGLWrapper::SetSpotLightNumber(u32 _number) const { SetUniform("nbSpotLights", _number); }

void Core::Renderer::OpenGLWrapper::SetLight(const LowRenderer::Lights::DirectionalLight* _p_light, u32 _index, Maths::Vec3 _direction) const
{
	//pbr Version

	//SetUniform(std::string("dirLights[" + _index) + "]" + ".direction", _direction);
	//SetUniform(std::string("dirLights[" + _index) + "]" + ".color", _p_newLight->color_);
	//SetUniform(std::string("dirLights[" + _index) + "]" + ".strength", _p_newLight->strength_);
	//SetUniform(std::string("dirLights[" + _index) + "]" + ".shadow", _p_newLight->bShadow_);
	//if (_p_newLight->bShadow_)
	//	SetUniform(std::string("u_dirLights[" + _index) + "]" + ".bias", _p_newLight->bias_);

	//phong Version
	SetUniform(std::string("dirLights[") + std::to_string(_index) + "]" + ".direction", _direction);
	SetUniform(std::string("dirLights[") + std::to_string(_index) + "]" + ".ambient", (_p_light->color_.ToVector3() * 0.2f));
	SetUniform(std::string("dirLights[") + std::to_string(_index) + "]" + ".diffuse", (_p_light->color_.ToVector3() * 0.8f));
	SetUniform(std::string("dirLights[") + std::to_string(_index) + "]" + ".specular", (_p_light->color_.ToVector3() * 1.0f));
}

void Core::Renderer::OpenGLWrapper::SetLight(const LowRenderer::Lights::PointLight* _p_light, u32 _index, Maths::Vec3 _position) const
{
	//pbr Version
//p_rhi->SetUniform(std::string("pointLights[" + _index) + "]" + ".position", _position);
//p_rhi->SetUniform(std::string("pointLights[" + _index) + "]" + ".color", _p_light->color_);
//p_rhi->SetUniform(std::string("pointLights[" + _index) + "]" + ".strength", _p_light->strength_);
//p_rhi->SetUniform(std::string("pointLights[" + _index) + "]" + ".shadow", _p_light->bShadow_);
//if (_p_light->bShadow_)
//	p_rhi->SetUniform(std::string("u_pointlights[" + _index) + "]" + ".bias", _p_light->bias_);

//phong Version
	SetUniform(std::string("pointLights[") + std::to_string(_index) + "]" + ".position", _position);
	SetUniform(std::string("pointLights[") + std::to_string(_index) + "]" + ".ambient", (_p_light->color_.ToVector3() * 0.2f));
	SetUniform(std::string("pointLights[") + std::to_string(_index) + "]" + ".diffuse", (_p_light->color_.ToVector3() * 0.8f));
	SetUniform(std::string("pointLights[") + std::to_string(_index) + "]" + ".specular", (_p_light->color_.ToVector3() * 1.0f));
	SetUniform(std::string("pointLights[") + std::to_string(_index) + "]" + ".constant", _p_light->constant_);
	SetUniform(std::string("pointLights[") + std::to_string(_index) + "]" + ".linear", _p_light->linear_);
	SetUniform(std::string("pointLights[") + std::to_string(_index) + "]" + ".quadratic", _p_light->quadratic_);
}

void Core::Renderer::OpenGLWrapper::SetLight(const LowRenderer::Lights::SpotLight* _p_light, u32 _index, Maths::Vec3 _position, Maths::Quat _rotation) const
{
	//pbr Version
	//SetUniform(std::string("spotLights[" + _index) + "]" + ".position", _position);
	//SetUniform(std::string("spotLights[" + _index) + "]" + ".color", _p_light->color_);
	//SetUniform(std::string("spotLights[" + _index) + "]" + ".strength", _p_light->strength_);
	//SetUniform(std::string("spotLights[" + _index) + "]" + ".shadow", _p_light->bShadow_);
	//SetUniform(std::string("spotLights[" + _index) + "]" + ".iCutOff", _p_light->inCutOff_);
	//SetUniform(std::string("spotLights[" + _index) + "]" + ".oCutOff", _p_light->outCutOff_);
	//if (_p_light->bShadow_)
	//	SetUniform(std::string("spotLights[" + _index) + "]" + ".bias", _p_light->bias_);

	//phong Version
	SetUniform(std::string("spotLights[") + std::to_string(_index) + "]" + ".position", _position);
	Maths::Vec3 direction = _rotation.RotateVector(Maths::Vec3::FORWARD);
	SetUniform(std::string("spotLights[") + std::to_string(_index) + "]" + ".direction", direction);
	SetUniform(std::string("spotLights[") + std::to_string(_index) + "]" + ".ambient", (_p_light->color_.ToVector3() * 0.2f));
	SetUniform(std::string("spotLights[") + std::to_string(_index) + "]" + ".diffuse", (_p_light->color_.ToVector3() * 0.8f));
	SetUniform(std::string("spotLights[") + std::to_string(_index) + "]" + ".specular", (_p_light->color_.ToVector3() * 1.0f));
	SetUniform(std::string("spotLights[") + std::to_string(_index) + "]" + ".cutOff", cos(_p_light->inCutOff_ / 2.f * Maths::Constants::DEG2RAD));
	SetUniform(std::string("spotLights[") + std::to_string(_index) + "]" + ".outerCutOff", cos(_p_light->outCutOff_ / 2.f * Maths::Constants::DEG2RAD));
	SetUniform(std::string("spotLights[") + std::to_string(_index) + "]" + ".constant", _p_light->constant_);
	SetUniform(std::string("spotLights[") + std::to_string(_index) + "]" + ".linear", _p_light->linear_);
	SetUniform(std::string("spotLights[") + std::to_string(_index) + "]" + ".quadratic", _p_light->quadratic_);
}
#pragma endregion

/*
	RESOURCES FUNCTIONS
*/
#pragma region RESOURCES
/*
	MATERIAL FUNCTIONS
*/
#pragma region MATERIAL
void Core::Renderer::OpenGLWrapper::LoadResource(Resources::MaterialPtr _p_material) { /*EMPTY*/ }

void Core::Renderer::OpenGLWrapper::UseResource(const Resources::MaterialPtr _p_material)
{
	if (_p_material->type_ == Resources::MaterialType::Unset)
	{
		DEBUG_ERROR("Material is not set !");
		return; // return if material not set to prevent a crash
	}
	if (_p_material->p_shader_->ID != shaderInUse_)
	{
		glUseProgram((GLuint)_p_material->p_shader_->ID);
		shaderInUse_ = (unsigned)_p_material->p_shader_->ID;
	}

	if (!_p_material->diffuse_)
		return; // return if diffuse_ not set to prevent a crash

	_p_material->diffuse_->Use(Resources::TextureType::Diffuse);
	if (_p_material->type_ == Resources::MaterialType::Unlit)
		return;//Unlit Version

	_p_material->specular_->Use(Resources::TextureType::Specular);
	SetUniform("gammaCorrection", _p_material->gammaCorrection_);

	if (_p_material->type_ == Resources::MaterialType::PBR)
	{
		//PBR Version
		_p_material->ao_->Use(Resources::TextureType::AO);
		_p_material->normal_->Use(Resources::TextureType::Normal);
		_p_material->roughness_->Use(Resources::TextureType::Roughness);
	}
	else if (_p_material->type_ == Resources::MaterialType::Phong)
	{
		//Phong Version
		SetUniform("material.shininess", _p_material->shininess_);
	}
}

void Core::Renderer::OpenGLWrapper::UseResource(const Resources::MaterialPtr _p_material, const int _shaderProgramIndex)
{
	if (_p_material->type_ == Resources::MaterialType::Unset)
		DEBUG_ERROR("Material is not set !")
		if (_shaderProgramIndex != shaderInUse_)
		{
			glUseProgram(_shaderProgramIndex);
			shaderInUse_ = _shaderProgramIndex;
		}
	_p_material->diffuse_->Use(Resources::TextureType::Diffuse);

	if (_p_material->type_ == Resources::MaterialType::Unlit)
		return;//Unlit Version

	_p_material->specular_->Use(Resources::TextureType::Specular);
	SetUniform(_shaderProgramIndex, "gammaCorrection", _p_material->gammaCorrection_);

	if (_p_material->type_ == Resources::MaterialType::PBR)
	{
		//PBR Version
		_p_material->ao_->Use(Resources::TextureType::AO);
		_p_material->normal_->Use(Resources::TextureType::Normal);
		_p_material->roughness_->Use(Resources::TextureType::Roughness);
	}
	else if (_p_material->type_ == Resources::MaterialType::Phong)
	{
		//Phong Version
		SetUniform(_shaderProgramIndex, "material.shininess", _p_material->shininess_);
	}
}

void Core::Renderer::OpenGLWrapper::UnloadResource(const Resources::MaterialPtr _p_material) { /*EMPTY*/ }

#pragma endregion

/*
	TEXTURE FUNCTIONS
*/
#pragma region TEXTURE
void Core::Renderer::OpenGLWrapper::LoadResource(Resources::TexturePtr _p_texture)
{
	GLuint newID;
	glGenTextures(1, &newID);
	_p_texture->ID = newID;

	GLenum format = GL_DEBUG_TYPE_ERROR;
	if (_p_texture->nrComponents_ == 1)
		format = GL_RED;
	else if (_p_texture->nrComponents_ == 3)
		format = GL_RGB;
	else if (_p_texture->nrComponents_ == 4)
		format = GL_RGBA;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, newID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, _p_texture->width_, _p_texture->height_, 0, format, GL_UNSIGNED_BYTE, _p_texture->data_);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	_p_texture->bLoaded_ = true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO find a better place for this
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.hpp"
unsigned int Core::Renderer::OpenGLWrapper::LoadCubemap(std::vector<std::string> faces)
{
	glGenTextures(1, &cubeMap_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap_);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenVertexArrays(1, &skyboxVAO_);
	glGenBuffers(1, &skyboxVBO_);
	static float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	//Todo make struct, maybe in Lowrenderer

	glBindVertexArray(skyboxVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO_);
	// load data into vertex buffers
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	return cubeMap_;
}
void Core::Renderer::OpenGLWrapper::RenderCubeMap()
{

	glBindVertexArray(skyboxVAO_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap_);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glActiveTexture(0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Core::Renderer::OpenGLWrapper::UseResource(const Resources::TexturePtr _p_texture)
{
	glActiveTexture((GL_TEXTURE0 + (int)_p_texture->textureType)); // active proper texture unit before binding
	if (_p_texture->textureType != Resources::TextureType::Unset)
		SetUniform(Resources::TextureTypeToString(_p_texture->textureType), (int)_p_texture->textureType);
	glBindTexture(GL_TEXTURE_2D, (GLuint)_p_texture->ID);
	glActiveTexture(0);
}
void Core::Renderer::OpenGLWrapper::UseResource(const Resources::TexturePtr _p_texture, Resources::TextureType _type)
{
	glActiveTexture((GL_TEXTURE0 + (int)_type)); // active proper texture unit before binding
	if (_p_texture->textureType != Resources::TextureType::Unset)
		SetUniform(Resources::TextureTypeToString(_type), (int)_type);
	glBindTexture(GL_TEXTURE_2D, (GLuint)_p_texture->ID);
	glActiveTexture(0);
}
void Core::Renderer::OpenGLWrapper::StopUseTexture() { glBindTexture(GL_TEXTURE_2D, 0); }

void Core::Renderer::OpenGLWrapper::UnloadResource(const Resources::TexturePtr _p_texture)
{
	GLuint deleteID =(GLuint)_p_texture->resourceId_;
	glDeleteTextures(1, &deleteID);
	_p_texture->resourceId_ = -1;
	_p_texture->bLoaded_ = false;
}

#pragma endregion

/*
	SHADER FUNCTIONS
*/
#pragma region SHADER
void Core::Renderer::OpenGLWrapper::LoadResource(Resources::ShaderPtr _p_shader)
{
	unsigned vertexID = -1, fragmentID = -1;
	{
		const char* vertexCode = _p_shader->vertexCode_.c_str();

		vertexID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexID, 1, &vertexCode, NULL);
		glCompileShader(vertexID);
		if (CheckCompileErrors(vertexID, "VERTEX")) // Vertex shader failled
			return;

		const char* fragmentCode = _p_shader->fragmentCode_.c_str();

		fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentID, 1, &fragmentCode, NULL);
		glCompileShader(fragmentID);
		if (CheckCompileErrors(fragmentID, "FRAGMENT")) // Fragment shader failled
			return;
	}
	unsigned shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexID);
	glAttachShader(shaderProgram, fragmentID);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);

	bool programFailed = CheckCompileErrors(shaderProgram, "LINKING");
	if (programFailed) return;

	_p_shader->ID = shaderProgram;
	_p_shader->bLoaded_ = !programFailed;
}

bool Core::Renderer::OpenGLWrapper::CheckCompileErrors(unsigned& _shaderID, std::string _type)
{
	int success; char infoLog[1024];
	if (_type != "LINKING")
	{
		glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(_shaderID, 1024, NULL, infoLog);
			DEBUG_ERROR("SHADER COMPILATION ERROR of type: %s \n %s \n",
				_type.c_str(), infoLog);
			return 1;
		}
	}
	else
	{
		glGetProgramiv(_shaderID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(_shaderID, 1024, NULL, infoLog);
			DEBUG_ERROR("PROGRAM LINKING ERROR of type: %s \n %s \n",
				_type.c_str(), infoLog);
			return 1;
		}
	}
	return 0;
}

void Core::Renderer::OpenGLWrapper::UseResource(const Resources::ShaderPtr _p_shader)
{
	if (_p_shader)
	{
		if (_p_shader->ID == -1)
		{
			DEBUG_WARNING("Cannot use Shader %s: ID = -1", _p_shader->name.c_str());
			return;
		}

		glUseProgram((GLuint)_p_shader->ID);
		shaderInUse_ = (unsigned)_p_shader->ID;
	}
}

void Core::Renderer::OpenGLWrapper::StopUseShader()
{
	glUseProgram(0);
	shaderInUse_ = -1;
}

void Core::Renderer::OpenGLWrapper::AddPostprocessShader(const Resources::ShaderPtr _p_shader) { /*TODO*/ }

void Core::Renderer::OpenGLWrapper::UnloadResource(const Resources::ShaderPtr _p_shader)
{
	if (shaderInUse_ != static_cast<unsigned>(-1) && _p_shader->ID == shaderInUse_)
		StopUseShader();
	glDeleteProgram((GLuint)_p_shader->ID);
	_p_shader->ID = -1;
	_p_shader->bLoaded_ = false;
}

#pragma endregion

/*
	MESH FUNCTIONS
*/
#pragma region MESH
void Core::Renderer::OpenGLWrapper::LoadResource(Resources::MeshPtr _p_mesh)
{
	for (Resources::BasicMesh& subMesh : _p_mesh->subMeshes_)
	{
		// create buffers/arrays
		glGenVertexArrays(1, &subMesh.VAO_);
		glGenBuffers(1, &subMesh.VBO_);
		glGenBuffers(1, &subMesh.EBO_);

		glBindVertexArray(subMesh.VAO_);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, subMesh.VBO_);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, subMesh.vertices_.size() * sizeof(Resources::Vertex), &subMesh.vertices_[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh.EBO_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, subMesh.indices_.size() * sizeof(unsigned int), &subMesh.indices_[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (void*)offsetof(Resources::Vertex, Resources::Vertex::normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (void*)offsetof(Resources::Vertex, Resources::Vertex::texCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (void*)offsetof(Resources::Vertex, Resources::Vertex::tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (void*)offsetof(Resources::Vertex, Resources::Vertex::bitangent));
		// ids
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Resources::Vertex), (void*)offsetof(Resources::Vertex, Resources::Vertex::boneIDs));
		// weights
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (void*)offsetof(Resources::Vertex, Resources::Vertex::weights));
		glBindVertexArray(0);
	}
	_p_mesh->bLoaded_ = true;
}

void Core::Renderer::OpenGLWrapper::UseResource(const Resources::MeshPtr _p_mesh)
{
	for (unsigned int i = 0; i < _p_mesh->subMeshes_.size(); i++)
	{
		glBindVertexArray(_p_mesh->subMeshes_[i].VAO_);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(_p_mesh->subMeshes_[i].indices_.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void Core::Renderer::OpenGLWrapper::UseResource(const Resources::MeshPtr _p_mesh, std::vector<Resources::MaterialPtr> p__materials)
{
	for (unsigned int i = 0; i < _p_mesh->subMeshes_.size(); i++)
	{
		if (i < p__materials.size())
			UseResource(p__materials[i]);
		glBindVertexArray(_p_mesh->subMeshes_[i].VAO_);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(_p_mesh->subMeshes_[i].indices_.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void Core::Renderer::OpenGLWrapper::UnloadResource(const Resources::MeshPtr _p_mesh) { /*TODO*/ }

#pragma endregion

#pragma endregion RESOURCES

/*
	UNIFORMS
*/
#pragma region UNIFORMS
void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, bool _value) const
{
	int valueLocation = static_cast<bool>(glGetUniformLocation(_shaderProgramIndex, _name.c_str()));
	glUniform1i(valueLocation, _value);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, bool _value) const
{
	int valueLocation = static_cast<bool>(glGetUniformLocation(shaderInUse_, _name.c_str()));
	glUniform1i(valueLocation, _value);
}

void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, int _value) const
{
	int valueLocation = glGetUniformLocation(_shaderProgramIndex, _name.c_str());
	glUniform1i(valueLocation, _value);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, int _value) const
{
	int valueLocation = glGetUniformLocation(shaderInUse_, _name.c_str());
	glUniform1i(valueLocation, _value);
}

void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, unsigned _value) const
{
	int valueLocation = glGetUniformLocation(_shaderProgramIndex, _name.c_str());
	glUniform1ui(valueLocation, _value);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, unsigned _value) const
{
	int valueLocation = glGetUniformLocation(shaderInUse_, _name.c_str());
	glUniform1ui(valueLocation, _value);
}

void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, float _value) const
{
	int valueLocation = glGetUniformLocation(_shaderProgramIndex, _name.c_str());
	glUniform1f(valueLocation, _value);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, float _value) const
{
	int valueLocation = glGetUniformLocation(shaderInUse_, _name.c_str());
	glUniform1f(valueLocation, _value);
}

void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec2 _value) const
{
	SetUniform(_shaderProgramIndex, _name, _value.x, _value.y);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, Maths::Vec2 _value) const
{
	SetUniform(shaderInUse_, _name, _value.x, _value.y);
}

void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY) const
{
	int valueLocation = glGetUniformLocation(_shaderProgramIndex, _name.c_str());
	glUniform2f(valueLocation, _valueX, _valueY);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, float _valueX, float _valueY) const
{
	int valueLocation = glGetUniformLocation(shaderInUse_, _name.c_str());
	glUniform2f(valueLocation, _valueX, _valueY);
}

void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec3 _value) const
{
	SetUniform(_shaderProgramIndex, _name, _value.x, _value.y, _value.z);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, Maths::Vec3 _value) const
{
	SetUniform(shaderInUse_, _name, _value.x, _value.y, _value.z);
}

void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY, float _valueZ) const
{
	int valueLocation = glGetUniformLocation(_shaderProgramIndex, _name.c_str());
	glUniform3f(valueLocation, _valueX, _valueY, _valueZ);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, float _valueX, float _valueY, float _valueZ) const
{
	int valueLocation = glGetUniformLocation(shaderInUse_, _name.c_str());
	glUniform3f(valueLocation, _valueX, _valueY, _valueZ);
}

void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Vec4 _value) const
{
	SetUniform(_shaderProgramIndex, _name, _value.x, _value.y, _value.z, _value.w);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, Maths::Vec4 _value) const
{
	SetUniform(shaderInUse_, _name, _value.x, _value.y, _value.z, _value.w);
}

void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, TNCColor _value) const
{
	SetUniform(_shaderProgramIndex, _name, _value.r, _value.g, _value.b, _value.a);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, TNCColor _value) const
{
	SetUniform(shaderInUse_, _name, _value.r, _value.g, _value.b, _value.a);
}

void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, float _valueX, float _valueY, float _valueZ, float _valueW) const
{
	int valueLocation = glGetUniformLocation(_shaderProgramIndex, _name.c_str());
	glUniform4f(valueLocation, _valueX, _valueY, _valueZ, _valueZ);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, float _valueX, float _valueY, float _valueZ, float _valueW) const
{
	int valueLocation = glGetUniformLocation(shaderInUse_, _name.c_str());
	glUniform4f(valueLocation, _valueX, _valueY, _valueZ, _valueZ);
}

void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Mat3 _value) const
{
	int valueLocation = glGetUniformLocation(_shaderProgramIndex, _name.c_str());
	glUniformMatrix3fv(valueLocation, 1, GL_FALSE, &_value.data[0]);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, Maths::Mat3 _value) const
{
	int valueLocation = glGetUniformLocation(shaderInUse_, _name.c_str());
	glUniformMatrix3fv(valueLocation, 1, GL_FALSE, &_value.data[0]);
}

void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, Maths::Mat4 _value) const
{
	int valueLocation = glGetUniformLocation(_shaderProgramIndex, _name.c_str());
	glUniformMatrix4fv(valueLocation, 1, GL_FALSE, &_value.data[0]);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, Maths::Mat4 _value) const
{
	int valueLocation = glGetUniformLocation(shaderInUse_, _name.c_str());
	glUniformMatrix4fv(valueLocation, 1, GL_FALSE, &_value.data[0]);
}
#pragma endregion