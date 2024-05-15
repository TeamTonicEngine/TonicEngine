#pragma once

#include "DLL_API.hpp"

#include "IResource.hpp"

#include "Maths/Maths.hpp"

namespace Core::Renderer { class OpenGLWrapper; }; // Needed for friend class

namespace Resources
{
	constexpr auto MAX_BONE_INFLUENCE = 4;

	struct Vertex
	{
		// position
		Maths::Vec3 position;
		// normal
		Maths::Vec3 normal;
		// texCoords
		Maths::Vec2 texCoords;
		// tangent
		Maths::Vec3 tangent;
		// bitangent
		Maths::Vec3 bitangent;
		// bone indexes which will influence this vertex
		int boneIDs[MAX_BONE_INFLUENCE];
		// weights from each bone
		float weights[MAX_BONE_INFLUENCE];
	};

	using std::vector;
	struct BasicMesh
	{
		vector<Vertex> vertices_;
		vector<unsigned int> indices_;
		unsigned VAO_ = (unsigned)-1, VBO_ = (unsigned)-1, EBO_ = (unsigned)-1;
	};

	class Mesh : public IResource, public std::enable_shared_from_this<Mesh>
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		vector<BasicMesh> subMeshes_;
		//vector<Resources::Texture> textures_;

		const void* scene_ = nullptr; // assimp requiried
		//std::string directory;

		friend class Core::Renderer::OpenGLWrapper;
		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		TONIC_ENGINE_API Mesh();

		void TONIC_ENGINE_API Destroy() override;

		void TONIC_ENGINE_API ReadFile(const fs::path _path) override;
		void TONIC_ENGINE_API LoadFile() override;

		void TONIC_ENGINE_API MetaWriteFile(const string _name) override {};
		void TONIC_ENGINE_API MetaReadFile(const string _name) override {};

		void TONIC_ENGINE_API ResourceUnload() override {};

		void TONIC_ENGINE_API Use() override;
		void TONIC_ENGINE_API Use(std::vector <Resources::MaterialPtr> _p_materials);

	private:
		void TONIC_ENGINE_API ProcessNode(void* _node);
		Resources::BasicMesh TONIC_ENGINE_API ProcessSubMesh(void* _mesh);
	};
	using MeshPtr = std::shared_ptr<Mesh>;
}