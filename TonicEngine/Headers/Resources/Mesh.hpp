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
		unsigned VAO_ = -1, VBO_ = -1, EBO_ = -1;
	};

	class Mesh : public IResource
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
		TONIC_ENGINE_API ~Mesh();

		void TONIC_ENGINE_API ReadFile(const string _name) override;
		void TONIC_ENGINE_API ReadFile(const fs::path _path) override;

		void TONIC_ENGINE_API MetaWriteFile(const string _name) override {};
		void TONIC_ENGINE_API MetaReadFile(const string _name) override {};

		void TONIC_ENGINE_API ResourceUnload() override {};

		void TONIC_ENGINE_API Use() const override;

	private:
		void TONIC_ENGINE_API ProcessNode(void* _node);
		Resources::BasicMesh TONIC_ENGINE_API ProcessSubMesh(void* _mesh);
	};
}