#include "Core/EngineOverride/GL_RHI.hpp"
#include <stdexcept>

#include <glad/glad.hpp>
#include <filesystem>
#include "TonicEngine/Core/Log.hpp"


// SHADER ID IN OPENGL IS PROGRAM IN OUR CODE !!!

GL_RHI::GL_RHI()
{
    
}
GL_RHI::~GL_RHI()
{
    delete shader_["BasicShader"];
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

    std::filesystem::path currentPath = std::filesystem::current_path();

    std::filesystem::path vertexPath = currentPath;
    std::filesystem::path fragmentPath = currentPath;

    // Vertex and Fragment shaders Path
    vertexPath += "\\Assets\\Shaders\\basicVertex.shader";
    fragmentPath += "\\Assets\\Shaders\\basicFragment.shader";
    Shader* shader = new Shader(vertexPath, fragmentPath);
    shader_["BasicShader"] = shader;
}

void GL_RHI::StartFrame(FreeCamera* _camera)
{
    glClearColor(109.f / 255.f, 7.f / 255.f, 26.f / 255.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind textures on corresponding texture units
    /*
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shader_["BasicShader"]->texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shader_["BasicShader"]->texture2);
    */

    Transform(_camera);
}

void GL_RHI::Transform(FreeCamera* _camera)
{
    ShaderUse("BasicShader");
    _camera->width = width_;
    _camera->height = height_;


    Maths::Mat4 model(1.0f);
    Maths::Mat4 view(1.0f);
    Maths::Mat4 projection(1.0f);

    projection = _camera->projection;
    //projection = Maths::Matrices::Perspective(45.0f * Maths::Constants::DEG2RAD, (float)width_ / (float)height_, 0.1f, 100.0f);
    SetMat4("BasicShader", "projection", projection);

    //view = Maths::Mat4::Translate(Maths::Vec3(0.0f, 0.0f, -3.0f));
    view = _camera->view;
    //view = Maths::Matrices::LookAt(_camera->eye, _camera->center, _camera->up);
    SetMat4("BasicShader", "view", view);

    model = Maths::Mat4::Translate(Maths::Vec3(0.0f, 0.0f, 0.0f));
    model = Maths::Mat4::Scale(Maths::Vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    SetMat4("BasicShader", "model", model);
}

void GL_RHI::EndFrame()
{
    glClearColor(109.f / 255.f, 7.f / 255.f, 26.f / 255.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GL_RHI::Draw()
{

    for (int j = 0; j < model_.size(); j++)
    {
       
        for (unsigned int i = 0; i < model_[j]->meshes.size(); i++)
        {
            // bind appropriate textures
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr = 1;
            unsigned int heightNr = 1;
            for (unsigned int k = 0; k < model_[j]->meshes[i].textures.size(); k++)
            {
                glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
                // retrieve texture number (the N in diffuse_textureN)
                string number;
                string name = model_[j]->meshes[i].textures[k].type;
                if (name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if (name == "texture_specular")
                    number = std::to_string(specularNr++); // transfer unsigned int to string
                else if (name == "texture_normal")
                    number = std::to_string(normalNr++); // transfer unsigned int to string
                else if (name == "texture_height")
                    number = std::to_string(heightNr++); // transfer unsigned int to string

                // now set the sampler to the correct texture unit
                glUniform1i(glGetUniformLocation(shader_["BasicShader"]->shaderProgram, (name + number).c_str()), i);
                // and finally bind the texture
                glBindTexture(GL_TEXTURE_2D, model_[j]->meshes[i].textures[k].ID);
            }

            // draw mesh
            glBindVertexArray(model_[j]->meshes[i].VAO);
            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(model_[j]->meshes[i].indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // always good practice to set everything back to defaults once configured.
            glActiveTexture(GL_TEXTURE0);
        }
    }
}

void GL_RHI::InitShader()
{
    Resources::FlipVertically(true);
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


    // --------------------------------------------------------------------- TEST ------------------------------------------------------------------------------------------------
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path modelPath = currentPath;
    modelPath += "\\Assets\\Model\\Back\\backpack.obj";
    GL_Model* model = new GL_Model(modelPath);
    model_.push_back(model);
    model_[0]->LoadModel(modelPath.string());
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

