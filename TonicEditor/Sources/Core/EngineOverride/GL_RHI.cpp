#include "Core/EngineOverride/GL_RHI.hpp"
#include <stdexcept>

#include <glad/glad.hpp>
#include <filesystem>
#include "TonicEngine/Core/Log.hpp"

Maths::Vec3 lightPositions[] = {
        Maths::Vec3(-10.0f,  10.0f, 10.0f),
        Maths::Vec3(10.0f,  10.0f, 10.0f),
        Maths::Vec3(-10.0f, -10.0f, 10.0f),
        Maths::Vec3(10.0f, -10.0f, 10.0f),
};
Maths::Vec3 lightColors[] = {
        Maths::Vec3(1.0f, 300.0f, 300.0f),
        Maths::Vec3(300.0f, 300.0f, 300.0f),
        Maths::Vec3(300.0f, 300.0f, 300.0f),
        Maths::Vec3(300.0f, 300.0f, 300.0f)
};
int nrRows = 7;
int nrColumns = 7;
float spacing = 2.5;

// SHADER ID IN OPENGL IS PROGRAM IN OUR CODE !!!

GL_RHI::GL_RHI()
{
    
}
GL_RHI::~GL_RHI()
{
    delete shader_["BasicShader"];
    delete soundClap;
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
    std::filesystem::path vertexpbrPath = currentPath;
    std::filesystem::path fragmentPath = currentPath;
    std::filesystem::path fragmentpbrPath = currentPath;

    std::filesystem::path soundPath = currentPath;

    // Vertex and Fragment shaders Path
    vertexPath += "\\Assets\\Shaders\\BASIC\\basicVertex.shader";
    vertexpbrPath += "\\Assets\\Shaders\\PBR\\pbrVertex.shader";
    fragmentPath += "\\Assets\\Shaders\\BASIC\\basicFragment.shader";
    fragmentpbrPath += "\\Assets\\Shaders\\PBR\\pbrFragment.shader";
    Shader* shader = new Shader(vertexPath, fragmentPath);
    Shader* pbrShader = new Shader(vertexpbrPath, fragmentpbrPath);
    shader_["BasicShader"] = shader;
    shader_["PbrShader"] = pbrShader;
    

    soundPath += "\\Assets\\Sound\\Applaudissements.wav";

    Sound* sound = new Sound(soundPath);
    soundClap = sound;

    soundClap->LoadSound();
    soundClap->Play();
}

void GL_RHI::StartFrame(FreeCamera* _camera, float _deltaTime)
{
    deltaTime = _deltaTime;
    glClearColor(109.f / 255.f, 7.f / 255.f, 26.f / 255.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Transform(_camera);
    camera = _camera;
}

void GL_RHI::Transform(FreeCamera* _camera)
{
    _camera->width = width_;
    _camera->height = height_;

    // SPHERE + MAT ----------------------------------------------------------------------------------------------
    ShaderUse("PbrShader");
    SetVec3("PbrShader", "albedo", 0.0f, 0.0f, 1.0f); // COLOR
    SetFloat("PbrShader", "ao", 1.0f);

    Maths::Mat4 model(1.0f);
    Maths::Mat4 view(1.0f);
    Maths::Mat4 projection(1.0f);

    projection = _camera->projection;
    SetMat4("PbrShader", "projection", projection);

    view = _camera->view;
    SetMat4("PbrShader", "view", view);
    SetVec3("PbrShader", "camPos", _camera->eye);

    // ----------------------------------- SPHERE ----------------------------------------------
    for (int row = 0; row < nrRows; ++row)
    {
        SetFloat("PbrShader", "metallic", (float)row / (float)nrRows);
        for (int col = 0; col < nrColumns; ++col)
        {
            SetFloat("PbrShader", "roughness", std::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

            model = Maths::Mat4(1.0f);
            model = Maths::Mat4::Translate(Maths::Vec3(
                (col - (nrColumns / 2)) * spacing, // X
                (row - (nrRows / 2)) * spacing, // Y
                -10.0f // Z
            ));
            SetMat4("PbrShader", "model", model);
            SetMat3("PbrShader", "normalMatrix", Maths::Mat3(model).Inverse().Transpose());
            RenderSphere();
        }
    }

    // ----------------------------------- LIGHTS ----------------------------------------------
    SetVec3("PbrShader", "albedo", 255.0f, 255.0f, 255.0f); // COLOR

    for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
    {
        Maths::Vec3 newPos = lightPositions[i] + Maths::Vec3((sin(deltaTime * 5.0) * 5.0), 0.0, -5.0);
        //newPos = lightPositions[i];
        SetVec3("PbrShader", "lightPositions[" + std::to_string(i) + "]", newPos);
        SetVec3("PbrShader", "lightColors[" + std::to_string(i) + "]", lightColors[i]);

        model = Maths::Mat4(1.0f);
        //model = Maths::Mat4::Translate(Maths::Vec3(0.0, 0.0, 5.0));
        model = Maths::Mat4::Scale(Maths::Vec3(0.5f));
        SetMat4("PbrShader", "model", model);
        SetMat3("PbrShader","normalMatrix", Maths::Mat4(model).Inverse().Transpose());
        RenderSphere();
    }
    

    // 3D MODEL ----------------------------------------------------------------------------------------------
    ShaderUse("BasicShader");

    model = Maths::Mat4(1.0f);
    view = Maths::Mat4(1.0f);
    projection = Maths::Mat4(1.0f);

    projection = _camera->projection;
    SetMat4("BasicShader", "projection", projection);

    view = _camera->view;
    SetMat4("BasicShader", "view", view);

    model = Maths::Mat4::Translate(Maths::Vec3(0.0f, 0.0f, -1.0f));
    //model = Maths::Mat4::Scale(Maths::Vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
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
                glActiveTexture(GL_TEXTURE0 + k); // active proper texture unit before binding
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
                glUniform1i(glGetUniformLocation(shader_["BasicShader"]->shaderProgram, (name + number).c_str()), k);
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
        /*
        */
    }
    
}

void GL_RHI::InitShader()
{
    Resources::FlipVertically(true);
    glEnable(GL_DEPTH_TEST);
    for(const auto& entry : shader_)
    {
        // build and compile our shader program
        // ------------------------------------
        // vertex shader
        const char* vcode = entry.second->vertexCode_.c_str();
        const char* fcode = entry.second->fragmentCode_.c_str();

        if (!glCreateShader)
        {
            DEBUG_WARNING("SHADERS NOT SUPPORTED");
        }
        else
            DEBUG_SUCCESS("SHADERS SUPPORTED");

        entry.second->vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(entry.second->vertexShader, 1, &vcode, NULL);
        glCompileShader(entry.second->vertexShader);

        // check for VERTEX shader compile errors
        CheckShaderCompileErrors(entry.second->vertexShader, "VERTEX");

        // fragment shader
        entry.second->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(entry.second->fragmentShader, 1, &fcode, NULL);
        glCompileShader(entry.second->fragmentShader);

        // check for FRAGMENT shader compile errors
        CheckShaderCompileErrors(entry.second->fragmentShader, "FRAGMENT");

        // link shaders
        entry.second->shaderProgram = glCreateProgram();
        glAttachShader(entry.second->shaderProgram, entry.second->vertexShader);
        glAttachShader(entry.second->shaderProgram, entry.second->fragmentShader);
        glLinkProgram(entry.second->shaderProgram);

        // check for linking errors
        CheckShaderCompileErrors(entry.second->shaderProgram, "PROGRAM");

        glDeleteShader(entry.second->vertexShader);
        glDeleteShader(entry.second->fragmentShader);
    }

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
    for (const auto& entry : shader_)
    {
        //Create Frame Buffer Object
        glGenFramebuffers(1, &entry.second->FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, entry.second->FBO);

        glGenTextures(1, &entry.second->texture_id);
        glBindTexture(GL_TEXTURE_2D, entry.second->texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, entry.second->texture_id, 0);

        //Create Render Buffer Object
        glGenRenderbuffers(1, &entry.second->RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, entry.second->RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, entry.second->RBO);

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

void GL_RHI::RenderSphere()
{
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<Maths::Vec3> positions;
        std::vector<Maths::Vec2> uv;
        std::vector<Maths::Vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(Maths::Vec3(xPos, yPos, zPos));
                uv.push_back(Maths::Vec2(xSegment, ySegment));
                normals.push_back(Maths::Vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = static_cast<unsigned int>(indices.size());

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
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

void GL_RHI::SetBool(const std::string& _shaderName, const std::string& _name, bool _value)
{
    glUniform1i(glGetUniformLocation(shader_[_shaderName]->shaderProgram, _name.c_str()), (int)_value);
}

void GL_RHI::SetInt(const std::string& _shaderName, const std::string& _name, int _value)
{
    glUniform1i(glGetUniformLocation(shader_[_shaderName]->shaderProgram, _name.c_str()), _value);
}

void GL_RHI::SetFloat(const std::string& _shaderName, const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(shader_[_shaderName]->shaderProgram, name.c_str()), value);
}

void GL_RHI::SetVec2(const std::string& _shaderName, const std::string& name, const Maths::Vec2& value)
{
    glUniform2fv(glGetUniformLocation(shader_[_shaderName]->shaderProgram, name.c_str()), 1, &value.x);
}

void GL_RHI::SetVec2(const std::string& _shaderName, const std::string& name, float x, float y)
{
    glUniform2f(glGetUniformLocation(shader_[_shaderName]->shaderProgram, name.c_str()), x, y);
}

void GL_RHI::SetVec3(const std::string& _shaderName, const std::string& name, const Maths::Vec3& value)
{
    glUniform3fv(glGetUniformLocation(shader_[_shaderName]->shaderProgram, name.c_str()), 1, &value.x);
}

void GL_RHI::SetVec3(const std::string& _shaderName, const std::string& name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(shader_[_shaderName]->shaderProgram, name.c_str()), x, y, z);
}

void GL_RHI::SetVec4(const std::string& _shaderName, const std::string& name, const Maths::Vec4& value)
{
    glUniform4fv(glGetUniformLocation(shader_[_shaderName]->shaderProgram, name.c_str()), 1, &value.x);
}

void GL_RHI::SetVec4(const std::string& _shaderName, const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(shader_[_shaderName]->shaderProgram, name.c_str()), x, y, z, w);
}

void GL_RHI::SetMat3(const std::string& _shaderName, const std::string& name, const Maths::Mat3& mat)
{
    glUniformMatrix3fv(glGetUniformLocation(shader_[_shaderName]->shaderProgram, name.c_str()), 1, GL_FALSE, &mat.data[0]);
}

void GL_RHI::SetMat4(const std::string& _shaderName, const std::string& name, const Maths::Mat4& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shader_[_shaderName]->shaderProgram, name.c_str()), 1, GL_FALSE, &mat.data[0]);
}

