#include "Core/EngineOverride/GL_RHI.hpp"
#include <stdexcept>

#include <glad/glad.hpp>
#include <filesystem>
#include "TonicEngine/Core/Log.hpp"

GL_RHI::GL_RHI()
{
    shader_ = new Shader();
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path vertexPath = currentPath;
    vertexPath += "\\Assets\\Shaders\\basicVertex.shader";
    std::filesystem::path fragmentPath = currentPath;
    fragmentPath += "\\Assets\\Shaders\\basicFragment.shader";
    shader_->Init(vertexPath, fragmentPath);

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
		std::cout << "SUCCEED TO INITIALIZE GLAD" << std::endl;
	}
}

void GL_RHI::StartFrame()
{
	glClearColor(109.f/255.f, 7.f/255.f, 26.f/255.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GL_RHI::DrawTriangle()
{
    // draw our first triangle
    glUseProgram(shader_->shaderProgram);
    glBindVertexArray(shader_->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0); // no need to unbind it every time 
    glUseProgram(0);
}

void GL_RHI::InitShader()
{
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    const char* vcode = shader_->vertexCode_.c_str();
    const char* fcode = shader_->fragmentCode_.c_str();

    shader_->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_->vertexShader, 1, &vcode, NULL);
    glCompileShader(shader_->vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader_->vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_->vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    shader_->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_->fragmentShader, 1, &fcode, NULL);
    glCompileShader(shader_->fragmentShader);
    // check for shader compile errors
    glGetShaderiv(shader_->fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_->fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    shader_->shaderProgram = glCreateProgram();
    glAttachShader(shader_->shaderProgram, shader_->vertexShader);
    glAttachShader(shader_->shaderProgram, shader_->fragmentShader);
    glLinkProgram(shader_->shaderProgram);
    // check for linking errors
    glGetProgramiv(shader_->shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_->shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(shader_->vertexShader);
    glDeleteShader(shader_->fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };
    
    glGenVertexArrays(1, &shader_->VAO);
    glGenBuffers(1, &shader_->VBO);
    glBindVertexArray(shader_->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, shader_->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void GL_RHI::InitFrameBuffer()
{
    glGenFramebuffers(1, &shader_->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shader_->FBO);

    glGenTextures(1, &shader_->texture_id);
    glBindTexture(GL_TEXTURE_2D, shader_->texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shader_->texture_id, 0);

    glGenRenderbuffers(1, &shader_->RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, shader_->RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, shader_->RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

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

void GL_RHI::RescaleFrameBuffer(float width, float height)
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
}