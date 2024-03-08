#include "Core/EngineOverride/GL_RHI.hpp"
#include <stdexcept>

#include <glad/glad.hpp>
#include <filesystem>
#include "TonicEngine/Core/Log.hpp"

GL_RHI::GL_RHI()
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path vertexPath = currentPath;
    vertexPath += "\\Assets\\Shaders\\basicVertex.shader";
    std::filesystem::path fragmentPath = currentPath;
    fragmentPath += "\\Assets\\Shaders\\basicFragment.shader";
    shader_ = new Shader(vertexPath, fragmentPath);
}
GL_RHI::~GL_RHI()
{
    delete shader_;
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
	glClearColor(109.f/255.f, 7.f/255.f, 26.f/255.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

    // bind textures on corresponding texture units
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shader_->texture1);
   // glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, shader_->texture2);
}

void GL_RHI::DrawTriangle()
{
    ShaderUse();
    glBindVertexArray(shader_->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GL_RHI::InitShader()
{
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    const char* vcode = shader_->vertexCode_.c_str();
    const char* fcode = shader_->fragmentCode_.c_str();

    if (!glCreateShader)
    {
        DEBUG_WARNING("SHADERS NOT SUPPORTED");
    }
    else
        DEBUG_SUCCESS("SHADERS SUPPORTED");

    shader_->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_->vertexShader, 1, &vcode, NULL);
    glCompileShader(shader_->vertexShader);

    // check for VERTEX shader compile errors
    CheckCompileErrors(shader_->vertexShader, "VERTEX");

    // fragment shader
    shader_->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_->fragmentShader, 1, &fcode, NULL);
    glCompileShader(shader_->fragmentShader);

    // check for FRAGMENT shader compile errors
    CheckCompileErrors(shader_->fragmentShader, "FRAGMENT");

    // link shaders
    shader_->shaderProgram = glCreateProgram();
    glAttachShader(shader_->shaderProgram, shader_->vertexShader);
    glAttachShader(shader_->shaderProgram, shader_->fragmentShader);
    glLinkProgram(shader_->shaderProgram);

    // check for linking errors
    CheckCompileErrors(shader_->shaderProgram, "PROGRAM");
    
    glDeleteShader(shader_->vertexShader);
    glDeleteShader(shader_->fragmentShader);

    InitShaderData();
}

void GL_RHI::CheckCompileErrors(unsigned int shader, std::string type)
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
            DEBUG_SUCCESS("SHADER COMPILED SUCCESSFULLY of type: %s", type);
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
			DEBUG_SUCCESS("PROGRAM LINKED SUCCESSFULLY of type: %s", type);
    }
}

void GL_RHI::InitShaderData()
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &shader_->VAO);
    glGenBuffers(1, &shader_->VBO);
    glGenBuffers(1, &shader_->EBO);

    glBindVertexArray(shader_->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, shader_->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader_->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create a texture 
    // -------------------------
    
    // texture 1
    // ---------
    glGenTextures(1, &shader_->texture1);
    glBindTexture(GL_TEXTURE_2D, shader_->texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    //shader_->FlipVertically(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path texturePath = currentPath;
    texturePath += "\\Assets\\Textures\\container.jpg";

    unsigned char* data = static_cast<unsigned char*>(shader_->LoadTexture(texturePath.string(), &width, &height, &nrChannels));

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    shader_->FreeImage(data);
}

void GL_RHI::InitFrameBuffer()
{
    //Create Frame Buffer Object
    glGenFramebuffers(1, &shader_->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shader_->FBO);

    glGenTextures(1, &shader_->texture_id);
    glBindTexture(GL_TEXTURE_2D, shader_->texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shader_->texture_id, 0);

    //Create Render Buffer Object
    glGenRenderbuffers(1, &shader_->RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, shader_->RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, shader_->RBO);

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
    glBindFramebuffer(GL_FRAMEBUFFER, shader_->FBO);
}

void GL_RHI::UnbindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GL_RHI::RescaleFrameBuffer(s32 width, s32 height)
{
    glBindTexture(GL_TEXTURE_2D, shader_->texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shader_->texture_id, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, shader_->RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, shader_->RBO);
}

void GL_RHI::ResetViewPort(float width, float height)
{
    glViewport(0, 0, width, height);
}

unsigned int GL_RHI::GetTextureID()
{
    return shader_->texture_id;
}

void GL_RHI::CleanUp()
{
    glDeleteVertexArrays(1, &shader_->VAO);
    glDeleteBuffers(1, &shader_->VBO);
    glDeleteBuffers(1, &shader_->EBO);
    glDeleteBuffers(1, &shader_->FBO);
    glDeleteBuffers(1, &shader_->RBO);
}

void GL_RHI::ShaderUse()
{
	glUseProgram(shader_->shaderProgram);
}

