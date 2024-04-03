#include "pch.hpp"

#include "Core/OpenGLWrapper.hpp"

#include <glad/glad.hpp>

#include "Core/Log.hpp"

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
	glEnable(GL_DEPTH_TEST);

	return true; // if this is reached Init succeeded
}

void Core::Renderer::OpenGLWrapper::InitFrameBuffer()
{
	//Create Frame Buffer Object
	glGenFramebuffers(1, &frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

	glGenTextures(1, &frameBufferID);
	glBindTexture(GL_TEXTURE_2D, frameBufferID);

	auto size = HEART::GetWindow()->GetScreenSize();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size[0], size[1], 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferID, 0);

	//Create Render Buffer Object
	glGenRenderbuffers(1, &renderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size[0], size[1]);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw ("FAILED TO INITIALIZE FRAMEBUFFER");
	else
		DEBUG_SUCCESS("INITIALIZED FRAMEBUFFER");

	//Unbind buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Core::Renderer::OpenGLWrapper::StartFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void Core::Renderer::OpenGLWrapper::Draw()
{
	/*********************************************/
			//	VARIABLES BLOC  //

	// Get Shaders
	auto whiteShader = GetRM()->Get<Resources::Shader>("white");
	auto lightShader = GetRM()->Get<Resources::Shader>("lighting_test");
	auto basicShader = GetRM()->Get<Resources::Shader>("basic");
	// Get Textures
	auto diffuseTest = GetRM()->Get<Resources::Texture>("backpack_diffuse");
	auto specTest = GetRM()->Get<Resources::Texture>("backpack_specular");
	auto normalTest = GetRM()->Get<Resources::Texture>("backpack_normal");
	// Get Meshes
	auto modelTest = GetRM()->Get<Resources::Mesh>("backpack");
	auto modelSphere = GetRM()->Get<Resources::Mesh>("sphere");

	Maths::Mat4 model(1.0f);
	Maths::Mat4 view = p_currentCamera_->view;
	Maths::Mat4 projection = p_currentCamera_->projection;
	Maths::Mat4 viewProj = p_currentCamera_->viewProjection;

	/**********************************************/

	// Set the Light Sphere SHADER -------------------------------------------
	whiteShader->Use();

	//p_currentCamera_->ComputeViewProjection();
	model = model.Translate(Maths::Vec3(0.f, 0.f, 10.f));
	SetUniform("model", model);
	SetUniform("view", view);
	SetUniform("projection", projection);

	// Use the Model Sphere
	modelSphere->Use();

	// Use Textures
	diffuseTest->Use();
	specTest->Use();
	normalTest->Use();

	// Set the Lighting SHADER -------------------------------------------
	lightShader->Use();
	model = Maths::Mat4(1.f);
	SetUniform("material.diffuse", GL_TEXTURE0 + diffuseTest->ID); // not for each frame
	SetUniform("material.specular", GL_TEXTURE0 + specTest->ID);// not for each frame
	SetUniform("material.shininess", 32.0f);

	//SetUniform("light.position", Maths::Vec3(0.f, 0.f, 10.f));
	SetUniform("viewPos", p_currentCamera_->eye);

	SetUniform("model", model);
	SetUniform("view", view);
	SetUniform("projection", projection);

	/* DON'T DELETE
	SetUniform("light.ambient", Maths::Vec3(0.2f, 0.2f, 0.2f));
	SetUniform("light.diffuse", Maths::Vec3(0.5f, 0.5f, 0.5f));
	SetUniform("light.specular", Maths::Vec3(1.0f, 1.0f, 1.0f));
	SetUniform("light.constant", 1.0f);
	SetUniform("light.linear", 0.014f);
	SetUniform("light.quadratic", 0.0007f);
	*/

	SetUniform("pointLights[0].position", Maths::Vec3(0.f, 0.f, 10.f));

	SetUniform("pointLights[0].constant", 1.0f);
	SetUniform("pointLights[0].linear", 0.014f);
	SetUniform("pointLights[0].quadratic", 0.0007f);

	SetUniform("pointLights[0].ambient", 0.2f, 0.2f, 0.2f);
	SetUniform("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	SetUniform("pointLights[0].specular", 1.0f, 1.0f, 1.0f);

	/*
	//SetUniform("MVP", viewProj);
	SetUniform("normalMatrix", Maths::Mat4(1.0f));

	SetUniform("objectColor", Maths::Vec3(1.0f, 1.0f, 1.0f));

	SetUniform("POINT_LIGHT_NBR", 1);
	SetUniform("DIRECTIONAL_LIGHT_NBR", 0);
	SetUniform("SPOT_LIGHT_NBR", 0);

	SetUniform("pointLight[0].position", sphereModel);

	SetUniform("pointLight[0].light.ambientColor", Maths::Vec3(0.2f));
	SetUniform("pointLight[0].light.diffuseColor", Maths::Vec3(0.5f));
	SetUniform("pointLight[0].light.specularColor", Maths::Vec3(1.f));

	SetUniform("pointLight[0].constant", 1.0f);
	SetUniform("pointLight[0].linear", 0.09f);
	SetUniform("pointLight[0].quadratic", 0.032f);
	//SetUniform("pointLight[0].light.ambientStrength", 25.f);
	//SetUniform("pointLight[0].light.specularStrength", .5f);

	SetUniform("material.diffuse2D", diffuseTest->ID); // not for each frame
	SetUniform("material.specular2D", specTest->ID);   // not for each frame
	//SetUniform("material.ambient", Maths::Vec3(.1f));
	//SetUniform("material.diffuse", Maths::Vec3(.5f));
	//SetUniform("material.specular", Maths::Vec3(1.0f));

	*/

	// Use the Models
	modelTest->Use();
}

void Core::Renderer::OpenGLWrapper::EndFrame() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Core::Renderer::OpenGLWrapper::BindFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject); }

void Core::Renderer::OpenGLWrapper::UnbindFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Core::Renderer::OpenGLWrapper::Destroy()
{
	glDisable(GL_DEPTH_TEST);
	glDeleteBuffers(1, &frameBufferObject);
	glDeleteBuffers(1, &renderBufferObject);
}

void Core::Renderer::OpenGLWrapper::ChangeClearColor(Color _color)
{
	clearColor_ = _color;
	f32_4 clear = _color.ToFloat();
	glClearColor(clear.x, clear.y, clear.z, clear.w);
}

void Core::Renderer::OpenGLWrapper::ClearColor()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Core::Renderer::OpenGLWrapper::ResizeViewPort(u32 width, u32 height)
{
	glViewport(0, 0, width, height);
}

void Core::Renderer::OpenGLWrapper::DeviceWaitIdle()
{
}

void Core::Renderer::OpenGLWrapper::FrameBufferResized()
{
	unsigned* size = p_window_->GetScreenSize();
	glViewport(0, 0, size[0], size[1]);
}

/*
	LOW RENDERER FUNCTIONS
*/
#pragma region LOW_RENDERER
void Core::Renderer::OpenGLWrapper::SetModel(Maths::Mat4 _modelMatrix)
{
}

//void Core::Renderer::OpenGLWrapper::SetCamera(Camera* _p_newCamera){} //

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

void Core::Renderer::OpenGLWrapper::SetLight(LowRenderer::ILight* _p_newLight)
{
}

void Core::Renderer::OpenGLWrapper::SetLights(std::vector<LowRenderer::ILight*> _lightList)
{
	for (LowRenderer::ILight* light : _lightList)
		SetLight(light);
}
#pragma endregion

/*
	RESOURCE FUNCTIONS
*/
//void LoadResource(Resources::IResource* _p_resource){}

//void UseResource(const Resources::IResource* _p_resource){}

//void UnloadResource(const Resources::IResource* _p_resource){}

/*
	MATERIAL FUNCTIONS
*/
#pragma region MATERIAL
void Core::Renderer::OpenGLWrapper::LoadMaterial(Resources::Material* _p_material)
{
}

void Core::Renderer::OpenGLWrapper::UseMaterial(const Resources::Material* _p_material)
{
}

void Core::Renderer::OpenGLWrapper::UnloadMaterial(const Resources::Material* _p_material)
{
}
#pragma endregion

/*
	TEXTURE FUNCTIONS
*/
#pragma region TEXTURE
void Core::Renderer::OpenGLWrapper::LoadTexture(Resources::Texture* _p_texture)
{
	GLuint newID;
	glGenTextures(1, &newID);
	_p_texture->ID = newID;

	GLenum format;
	if (_p_texture->nrComponents_ == 1)
		format = GL_RED;
	else if (_p_texture->nrComponents_ == 3)
		format = GL_RGB;
	else if (_p_texture->nrComponents_ == 4)
		format = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, _p_texture->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, _p_texture->width_, _p_texture->height_, 0, format, GL_UNSIGNED_BYTE, _p_texture->data_);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Core::Renderer::OpenGLWrapper::UseTexture(const Resources::Texture* _p_texture)
{
	glActiveTexture(GL_TEXTURE0 + _p_texture->ID); // active proper texture unit before binding
	if (_p_texture->texType_ != Resources::TextureType::Unset)
		SetUniform(Resources::TextureTypeToString(_p_texture->texType_), _p_texture->ID);
	glBindTexture(GL_TEXTURE_2D, _p_texture->ID);
	glActiveTexture(GL_TEXTURE0);
}

void Core::Renderer::OpenGLWrapper::StopUseTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Core::Renderer::OpenGLWrapper::UnloadTexture(const Resources::Texture* _p_texture)
{
}
#pragma endregion

/*
	SHADER FUNCTIONS
*/
#pragma region SHADER
void Core::Renderer::OpenGLWrapper::LoadShader(Resources::Shader* _p_shader)
{
	unsigned vertexID = -1, fragmentID = -1;

	std::string vertCode = Resources::ReadFileContent(_p_shader->GetVertexPath());
	const char* vertCodeChar = vertCode.c_str();

	vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vertCodeChar, NULL);
	glCompileShader(vertexID);
	if (CheckCompileErrors(vertexID, "VERTEX")) // Vertex shader failled
		return;

	std::string fragCode = Resources::ReadFileContent(_p_shader->GetFragmentPath());
	const char* fragCodeChar = fragCode.c_str();

	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fragCodeChar, NULL);
	glCompileShader(fragmentID);
	if (CheckCompileErrors(fragmentID, "FRAGMENT")) // Fragment shader failled
		return;

	//_p_shader->bRead_ = true;

	unsigned shaderProgram = glCreateProgram(); // RESOURCE ID TO SET
	glAttachShader(shaderProgram, vertexID);
	glAttachShader(shaderProgram, fragmentID);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);

	if (CheckCompileErrors(shaderProgram, "LINKING"))
		return;

	_p_shader->ID = shaderProgram;
	//_p_shader->bLoaded_ = true;
	DEBUG_SUCCESS("Shader program \"%s\" created #%i", _p_shader->name.c_str(), shaderProgram);
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

void Core::Renderer::OpenGLWrapper::UseShader(const Resources::Shader* _p_shader)
{
	if (_p_shader->ID != shaderInUse_)
	{
		glUseProgram(_p_shader->ID);
		shaderInUse_ = _p_shader->ID;
	}
}

void Core::Renderer::OpenGLWrapper::StopUseShader()
{
	glUseProgram(0);
	shaderInUse_ = -1;
}

void Core::Renderer::OpenGLWrapper::AddPostprocessShader(const Resources::Shader* _p_shader)
{
}

void Core::Renderer::OpenGLWrapper::UnloadShader(const Resources::Shader* _p_shader)
{
	if (shaderInUse_ != static_cast<unsigned>(-1) && _p_shader->ID == shaderInUse_)
		StopUseShader();
	glDeleteProgram(_p_shader->ID);
}
#pragma endregion

/*
	MESH FUNCTIONS
*/
#pragma region MESH
void Core::Renderer::OpenGLWrapper::LoadMesh(Resources::BasicMesh* _p_mesh)
{
	// create buffers/arrays
	glGenVertexArrays(1, &_p_mesh->VAO_);
	glGenBuffers(1, &_p_mesh->VBO_);
	glGenBuffers(1, &_p_mesh->EBO_);

	glBindVertexArray(_p_mesh->VAO_);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, _p_mesh->VBO_);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, _p_mesh->vertices_.size() * sizeof(Resources::Vertex), &_p_mesh->vertices_[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _p_mesh->EBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _p_mesh->indices_.size() * sizeof(unsigned int), &_p_mesh->indices_[0], GL_STATIC_DRAW);

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

void Core::Renderer::OpenGLWrapper::UseMesh(const Resources::Mesh* _p_mesh)
{
	for (unsigned int i = 0; i < _p_mesh->subMeshes_.size(); i++)
	{
		glBindVertexArray(_p_mesh->subMeshes_[i].VAO_);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(_p_mesh->subMeshes_[i].indices_.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void Core::Renderer::OpenGLWrapper::UnloadMesh(const Resources::Mesh* _p_mesh)
{
}
#pragma endregion

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

void Core::Renderer::OpenGLWrapper::SetUniform(const int _shaderProgramIndex, const std::string& _name, Color _value) const
{
	SetUniform(_shaderProgramIndex, _name, _value.r, _value.g, _value.b, _value.a);
}
void Core::Renderer::OpenGLWrapper::SetUniform(const std::string& _name, Color _value) const
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