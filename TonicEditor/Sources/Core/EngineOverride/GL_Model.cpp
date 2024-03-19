#include "Core/EngineOverride/GL_Model.hpp"
#include <glad/glad.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "../../../TonicEngine/Externals/include/STB/stb_image.hpp"

using namespace Resources;

GL_Model::GL_Model(std::filesystem::path const _modelPath, bool _gamma)
{

}

void GL_Model::Init(std::filesystem::path const _modelPath)
{
    LoadModel(_modelPath.string());
}

void GL_Model::LoadModel(std::string const& _path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
     //retrieve the directory path of the filepath
    directory = _path.substr(0, _path.find_last_of('/'));

     //process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
}

void GL_Model::ProcessNode(aiNode* _node, const aiScene* _scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < _node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, _scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < _node->mNumChildren; i++)
    {
        ProcessNode(_node->mChildren[i], _scene);
    }
}

GL_Mesh GL_Model::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
    {
        Vertex vertex;
        Maths::Vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = _mesh->mVertices[i].x;
        vector.y = _mesh->mVertices[i].y;
        vector.z = _mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (_mesh->HasNormals())
        {
            vector.x = _mesh->mNormals[i].x;
            vector.y = _mesh->mNormals[i].y;
            vector.z = _mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (_mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            Maths::Vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = _mesh->mTextureCoords[0][i].x;
            vec.y = _mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = _mesh->mTangents[i].x;
            vector.y = _mesh->mTangents[i].y;
            vector.z = _mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = _mesh->mBitangents[i].x;
            vector.y = _mesh->mBitangents[i].y;
            vector.z = _mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = Maths::Vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < _mesh->mNumFaces; i++)
    {
        aiFace face = _mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return GL_Mesh(vertices, indices, textures);
}

std::vector<Texture> GL_Model::LoadMaterialTextures(aiMaterial* _mat, aiTextureType _type, std::string _typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < _mat->GetTextureCount(_type); i++)
    {
        aiString str;
        _mat->GetTexture(_type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].texturePath_.string().data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.ID = TextureFromFile(str.C_Str(), this->directory);
            texture.type = _typeName;
            texture.texturePath_ = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}

unsigned int GL_Model::TextureFromFile(const char* _path, const std::string& _directory)
{
    string filename = string(_path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << _path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// -------------------------- MESH -------------------------------------------

void Resources::FlipVertically(bool _isOn)
{
    stbi_set_flip_vertically_on_load(_isOn);
}

void* Resources::LoadTexture(std::string _path, int* width, int* height, int* nrChannels)
{
    return stbi_load(_path.c_str(), width, height, nrChannels, 0);
}

void Resources::FreeImage(void* _data)
{
    stbi_image_free(_data);
}

GL_Mesh::GL_Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures)
{
    vertices = _vertices;
    indices = _indices;
    textures = _textures;

    SetupMesh();
}

void GL_Mesh::SetupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
    glBindVertexArray(0);
}