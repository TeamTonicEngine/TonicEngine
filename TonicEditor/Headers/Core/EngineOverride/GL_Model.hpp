#pragma once
#include "TonicEngine/Resources/Texture.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "TonicEngine/Core/Maths.hpp"
#define MAX_BONE_INFLUENCE 4
struct Vertex
{
	// position
	Maths::Vec3 Position;
	// normal
	Maths::Vec3 Normal;
	// texCoords
	Maths::Vec2 TexCoords;
	// tangent
	Maths::Vec3 Tangent;
	// bitangent
	Maths::Vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

class GL_Mesh
{
private:
public:
	// mesh Data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Resources::Texture>      textures;
	unsigned int VAO, VBO, EBO;

	GL_Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Resources::Texture> _textures);
	void SetupMesh();

};

class GL_Model
{
private:
public:
	std::vector<Resources::Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<GL_Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	GL_Model(std::filesystem::path const _modelPath, bool _gamma = false);

	//MODEL
	void Init(std::filesystem::path const _modelPath);
	void LoadModel(std::string const& _path);
	void ProcessNode(aiNode* _node, const aiScene* _scene);
	GL_Mesh ProcessMesh(aiMesh* _mesh, const aiScene* _scene);
	std::vector<Resources::Texture> LoadMaterialTextures(aiMaterial* _mat, aiTextureType _type, std::string _typeName);
	unsigned int TextureFromFile(const char* _path, const std::string& _directory);
};

namespace Resources
{
	void FlipVertically(bool _isOn);
	void* LoadTexture(std::string _path, int* width, int* height, int* nrChannels);
	void FreeImage(void* _data);
}

