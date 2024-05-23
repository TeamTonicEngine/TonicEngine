#include "pch.hpp"

#include "Resources/Mesh.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

Resources::Mesh::Mesh() { type_ = ResourceType::Mesh; }

void Resources::Mesh::Destroy() { ENGINE.RDR->UnloadResource(shared_from_this()); }

void Resources::Mesh::ReadFile(const fs::path _path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	p_scene_ = importer.ReadFile(_path.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// check for errors
	if (!p_scene_ || ((aiScene*)p_scene_)->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !((aiScene*)p_scene_)->mRootNode) // if is Not Zero
	{
		DEBUG_ERROR("Resources::Mesh::ReadFile assimp failed to load %s: %s", name.c_str(), importer.GetErrorString());
		return;
	}

	//process ASSIMP's root node recursively
	ProcessNode(((aiScene*)p_scene_)->mRootNode);
	bRead_ = true;
}

void Resources::Mesh::LoadFile()
{
	if (bRead_)
	{
		ENGINE.RDR->LoadResource(shared_from_this());
		if (bLoaded_)
			DEBUG_SUCCESS("Mesh %s loaded", name.c_str())
		else
			DEBUG_WARNING("Mesh failed loading: %s", name.c_str());
	}
	else
		DEBUG_WARNING("File not Read, cannot be loaded: %s", name.c_str());
}

void Resources::Mesh::Use()
{
	if (bLoaded_)
		ENGINE.RDR->UseResource(shared_from_this());
	else
	{
		LoadFile();
		if (bLoaded_)
			ENGINE.RDR->UseResource(shared_from_this());
	}
}

void Resources::Mesh::Use(std::vector<Resources::MaterialPtr> _p_materials)
{
	if (bLoaded_)
		ENGINE.RDR->UseResource(shared_from_this(), _p_materials);
	else
	{
		LoadFile();
		if (bLoaded_)
			ENGINE.RDR->UseResource(shared_from_this(), _p_materials);
	}
}

void Resources::Mesh::ProcessNode(void* _node) //aiNode* _node
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < ((aiNode*)_node)->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene.
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = ((aiScene*)p_scene_)->mMeshes[((aiNode*)_node)->mMeshes[i]];
		subMeshes_.push_back(ProcessSubMesh(mesh));
	}

	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < ((aiNode*)_node)->mNumChildren; i++)
		ProcessNode(((aiNode*)_node)->mChildren[i]);
}

Maths::Vec3 Vec3FromAssimp(aiVector3D assimpVec3) { return { assimpVec3.x, assimpVec3.y,  assimpVec3.z }; }

Resources::BasicMesh Resources::Mesh::ProcessSubMesh(void* _mesh) //aiMesh* _mesh
{
	// data to fill
	std::vector<Resources::Vertex> vertices;
	std::vector<unsigned int> indices;
	aiMesh* currentMesh = ((aiMesh*)_mesh);

	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < currentMesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.position = Vec3FromAssimp(currentMesh->mVertices[i]);;

		// normals
		if (currentMesh->HasNormals())
		{
			vertex.normal = Vec3FromAssimp(currentMesh->mNormals[i]);
		}
		// texture coordinates
		if (currentMesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			vertex.texCoords = Vec3FromAssimp(currentMesh->mTextureCoords[0][i]);
			// tangent
			vertex.tangent = Vec3FromAssimp(currentMesh->mTangents[i]);
			// bitangent
			vertex.bitangent = Vec3FromAssimp(currentMesh->mBitangents[i]);
		}
		else
			vertex.texCoords = Maths::Vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < currentMesh->mNumFaces; i++)
	{
		aiFace face = currentMesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = ((aiScene*)p_scene_)->mMaterials[currentMesh->mMaterialIndex];

	Resources::BasicMesh newMesh = { vertices, indices };
	return newMesh;
}