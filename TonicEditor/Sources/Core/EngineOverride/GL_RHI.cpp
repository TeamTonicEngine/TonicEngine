#include "Core/EngineOverride/GL_RHI.hpp"
#include <stdexcept>

#include <glad/glad.hpp>
#include <filesystem>
#include "TonicEngine/Core/Log.hpp"

// SHADER ID IN OPENGL IS PROGRAM IN OUR CODE !!!

GL_RHI::GL_RHI()
{
    
    std::filesystem::path currentPath = std::filesystem::current_path();

    std::filesystem::path vertexPath = currentPath;
    std::filesystem::path fragmentPath = currentPath;

    std::filesystem::path containerTexturePath = currentPath;
    std::filesystem::path faceTexturePath = currentPath;

    // Vertex and Fragment shaders Path
    vertexPath += "\\Assets\\Shaders\\basicVertex.shader";
    fragmentPath += "\\Assets\\Shaders\\basicFragment.shader";
    Shader* shader = new Shader(vertexPath, fragmentPath);

    // Container Texture
    containerTexturePath += "\\Assets\\Textures\\container.jpg";
    Texture* containerTexture = new Texture(containerTexturePath, "Container");

    // AwesomeFace Texture
    faceTexturePath += "\\Assets\\Textures\\awesomeface.jpg";
    Texture* faceTexture = new Texture(faceTexturePath, "AwesomeFace");

    // Shader map
    shader_["BasicShader"] = shader;

    // Texture map
    texture_.push_back(containerTexture);
    texture_.push_back(faceTexture);

    for (const auto &iter : texture_)
    {
        std::cout << iter->name << std::endl;
    }
}
GL_RHI::~GL_RHI()
{
    delete shader_["BasicShader"];
    delete texture_[0];
    delete texture_[1];
}

void GL_RHI::Init(uint32_t width, uint32_t height)
{
    width_ = width;
    height_ = height;

	if (!gladLoadGL())
	{
		throw std::exception("FAIL TO INITIALIZE GLAD");
	}
	else
	{
        DEBUG_SUCCESS("SUCCEED TO INITIALIZE GLAD");
	}
}

void GL_RHI::StartFrame()
{
    glClearColor(109.f / 255.f, 7.f / 255.f, 26.f / 255.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shader_["BasicShader"]->texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shader_["BasicShader"]->texture2);

    Transform();
}

void GL_RHI::Transform()
{
    ShaderUse("BasicShader");

    Maths::Mat4 model(1.0f);
    Maths::Mat4 view(1.0f);
    Maths::Mat4 projection(1.0f);

    model = Maths::Mat4::RotateX(-55.0f * Maths::Constants::DEG2RAD); // TODO : ADD TIME
    view = Maths::Mat4::Translate(Maths::Vec3(0.0f, 0.0f, -3.0f));
    projection = Maths::Matrices::Perspective(45.0f * Maths::Constants::DEG2RAD, (float)width_ / (float)height_, 0.1f, 100.0f);

    unsigned int modelLoc = glGetUniformLocation(shader_["BasicShader"]->shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shader_["BasicShader"]->shaderProgram, "view");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model.data[0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view.data[0]);

    SetMat4("BasicShader", "projection", projection);
}

void GL_RHI::EndFrame()
{
    glClearColor(109.f / 255.f, 7.f / 255.f, 26.f / 255.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GL_RHI::Draw()
{
    
    ShaderUse("BasicShader");
    glBindVertexArray(shader_["BasicShader"]->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void GL_RHI::InitShader()
{

    glEnable(GL_DEPTH_TEST);
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    const char* vcode = shader_["BasicShader"]->vertexCode_.c_str();
    const char* fcode = shader_["BasicShader"]->fragmentCode_.c_str();

    if (!glCreateShader)
    {
        DEBUG_WARNING("SHADERS NOT SUPPORTED");
    }
    else
        DEBUG_SUCCESS("SHADERS SUPPORTED");

    shader_["BasicShader"]->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_["BasicShader"]->vertexShader, 1, &vcode, NULL);
    glCompileShader(shader_["BasicShader"]->vertexShader);

    // check for VERTEX shader compile errors
    CheckShaderCompileErrors(shader_["BasicShader"]->vertexShader, "VERTEX");

    // fragment shader
    shader_["BasicShader"]->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_["BasicShader"]->fragmentShader, 1, &fcode, NULL);
    glCompileShader(shader_["BasicShader"]->fragmentShader);

    // check for FRAGMENT shader compile errors
    CheckShaderCompileErrors(shader_["BasicShader"]->fragmentShader, "FRAGMENT");

    // link shaders
    shader_["BasicShader"]->shaderProgram = glCreateProgram();
    glAttachShader(shader_["BasicShader"]->shaderProgram, shader_["BasicShader"]->vertexShader);
    glAttachShader(shader_["BasicShader"]->shaderProgram, shader_["BasicShader"]->fragmentShader);
    glLinkProgram(shader_["BasicShader"]->shaderProgram);

    // check for linking errors
    CheckShaderCompileErrors(shader_["BasicShader"]->shaderProgram, "PROGRAM");
    
    glDeleteShader(shader_["BasicShader"]->vertexShader);
    glDeleteShader(shader_["BasicShader"]->fragmentShader);

    InitShaderData();
}

void GL_RHI::CheckShaderCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            DEBUG_WARNING("ERROR::SHADER_COMPILATION_ERROR of type: %s \n %s \n -- -------------------------------------------------- - -- ", type, infoLog);
        }
        else
            DEBUG_SUCCESS("SHADER COMPILED SUCCESSFULLY of type: %s", type.c_str());
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            DEBUG_WARNING("ERROR::PROGRAM_LINKING_ERROR of type: %s \n %s \n -- -------------------------------------------------- - -- ", type, infoLog);
        }
        else
			DEBUG_SUCCESS("PROGRAM LINKED SUCCESSFULLY of type: %s", type.c_str());
    }
}

void GL_RHI::InitShaderData()
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &shader_["BasicShader"]->VAO);
    glGenBuffers(1, &shader_["BasicShader"]->VBO);
    //glGenBuffers(1, &shader_["BasicShader"]->EBO);

    glBindVertexArray(shader_["BasicShader"]->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, shader_["BasicShader"]->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create a texture 
    // -------------------------
    
    // texture 1
    // ---------
    glGenTextures(1, &shader_["BasicShader"]->texture1);
    glBindTexture(GL_TEXTURE_2D, shader_["BasicShader"]->texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    shader_["BasicShader"]->FlipVertically(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.

    unsigned char* data = static_cast<unsigned char*>(shader_["BasicShader"]->LoadTexture(texture_[0]->texturePath_.string(), &width, &height, &nrChannels));

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    shader_["BasicShader"]->FreeImage(data);

    // texture 2
    // ---------
    glGenTextures(1, &shader_["BasicShader"]->texture2);
    glBindTexture(GL_TEXTURE_2D, shader_["BasicShader"]->texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = static_cast<unsigned char*>(shader_["BasicShader"]->LoadTexture(texture_[1]->texturePath_.string(), &width, &height, &nrChannels));

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    shader_["BasicShader"]->FreeImage(data);

    ShaderUse("BasicShader");
    SetInt("BasicShader", "texture1", 0);
    SetInt("BasicShader", "texture2", 1);
}


void GL_RHI::InitFrameBuffer()
{
    //Create Frame Buffer Object
    glGenFramebuffers(1, &shader_["BasicShader"]->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shader_["BasicShader"]->FBO);

    glGenTextures(1, &shader_["BasicShader"]->texture_id);
    glBindTexture(GL_TEXTURE_2D, shader_["BasicShader"]->texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shader_["BasicShader"]->texture_id, 0);

    //Create Render Buffer Object
    glGenRenderbuffers(1, &shader_["BasicShader"]->RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, shader_["BasicShader"]->RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, shader_["BasicShader"]->RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        DEBUG_WARNING("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
    }
    else
        DEBUG_SUCCESS("Framebuffer is complete!\n");

    //Unbind buffers
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void GL_RHI::BindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, shader_["BasicShader"]->FBO);
}

void GL_RHI::UnbindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GL_RHI::RescaleFrameBuffer(s32 width, s32 height)
{
    glViewport(0, 0, width, height);
}

u32 GL_RHI::GetTextureID()
{
    return shader_["BasicShader"]->texture_id;
}

void GL_RHI::CleanUp()
{
    glDeleteVertexArrays(1, &shader_["BasicShader"]->VAO);
    glDeleteBuffers(1, &shader_["BasicShader"]->VBO);
    glDeleteBuffers(1, &shader_["BasicShader"]->EBO);
    glDeleteBuffers(1, &shader_["BasicShader"]->FBO);
    glDeleteBuffers(1, &shader_["BasicShader"]->RBO);
}

void GL_RHI::ShaderUse(std::string _shaderName)
{
	glUseProgram(shader_[_shaderName]->shaderProgram);
}

void GL_RHI::SetBool(const std::string& _name, bool _value) const
{

}

void GL_RHI::SetInt(const std::string& _shaderName, const std::string& _name, int _value)
{
    glUniform1i(glGetUniformLocation(shader_[_shaderName]->shaderProgram, _name.c_str()), _value);
}

void GL_RHI::SetFloat(const std::string& name, float value) const
{

}

void GL_RHI::SetVec2(const std::string& name, const Maths::Vec2& value) const
{

}

void GL_RHI::SetVec2(const std::string& name, float x, float y) const
{

}

void GL_RHI::SetVec3(const std::string& name, const Maths::Vec3& value) const
{

}

void GL_RHI::SetVec3(const std::string& name, float x, float y, float z) const
{

}

void GL_RHI::SetVec4(const std::string& name, const Maths::Vec4& value) const
{

}

void GL_RHI::SetVec4(const std::string& name, float x, float y, float z, float w)
{

}

void GL_RHI::SetMat3(const std::string& name, const Maths::Mat3& mat) const
{

}

void GL_RHI::SetMat4(const std::string& _shaderName, const std::string& name, const Maths::Mat4& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shader_[_shaderName]->shaderProgram, name.c_str()), 1, GL_FALSE, &mat.data[0]);
}