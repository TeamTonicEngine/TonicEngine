#pragma once

#include "pch.hpp"

#include <string>
#include <cstring>
#include <fstream>
#include <mutex>

#include <Core/Log.hpp>

namespace fs = std::filesystem;

namespace Core::Renderer { class OpenGLWrapper; }; // Needed for friend class

namespace Resources
{
	using std::string;

	enum class TONIC_ENGINE_API ResourceType
	{
		Unset,
		Shader,
		Texture,
		Mesh,
		Material,
		Sound,
		Physical,
		Scene,
		Script
	};

	class IResource
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		std::atomic<bool> bRead_ = false;
		std::atomic<bool> bLoaded_ = false;
		ResourceType type_ = ResourceType::Unset;

	private:
		u64 resourceId_ = -1;
		u64 resourceManagerId_ = -1;
		fs::path resourcePath_ = fs::path();
		string name_ = "";

	public:
		__declspec(property(get = GetResourceId, put = SetResourceId))
			u64 ID;
		__declspec(property(get = GetResourcePath, put = SetResourcePath))
			fs::path path;
		__declspec(property(get = GetResourceName, put = SetResourceName))
			string name;

		__declspec(property(get = GetResourceManagerId, put = SetResourceManagerId))
			u64 RMID;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		virtual TONIC_ENGINE_API ~IResource() = default;

		inline u64 TONIC_ENGINE_API GetResourceManagerId() const { return resourceManagerId_; }
		inline void TONIC_ENGINE_API SetResourceManagerId(u64 _id) { resourceManagerId_ = _id; }

	protected:
		virtual void TONIC_ENGINE_API Destroy() = 0; // Required because of shared_ptr shenanigans (either destructor loop or bad weak ptr)

	public:
		void TONIC_ENGINE_API ReadFileTimed(const fs::path _path);
		virtual void TONIC_ENGINE_API ReadFile(const fs::path _path) = 0;

		virtual void TONIC_ENGINE_API LoadFile() = 0;
		virtual void TONIC_ENGINE_API LoadFileForced() { do { LoadFile(); } while (!bLoaded_); };

		virtual void TONIC_ENGINE_API MetaWriteFile(const string _name) = 0;
		virtual void TONIC_ENGINE_API MetaReadFile(const string _name) = 0;

		virtual void TONIC_ENGINE_API Use() = 0;

		virtual void TONIC_ENGINE_API ResourceUnload() = 0;

		virtual bool TONIC_ENGINE_API IsReadNotLoaded();
		virtual bool TONIC_ENGINE_API IsLoaded();
		virtual void TONIC_ENGINE_API BypassLoad();

	protected:	// Properties get & set (see public vars)
		virtual void TONIC_ENGINE_API SetResourceId(const u64& _newId);
	public:
		virtual const u64 TONIC_ENGINE_API GetResourceId();

	protected:
		virtual void TONIC_ENGINE_API SetResourcePath(const fs::path& _newPath);
	public:
		virtual fs::path TONIC_ENGINE_API GetResourcePath() const;


	protected:
		virtual void TONIC_ENGINE_API SetResourceName(const string& _newName);
	public:
		virtual string TONIC_ENGINE_API GetResourceName() const;

	public:
		friend class ResourceManager;
		friend class Core::Renderer::OpenGLWrapper;
	};

	/* Output : Returns empty path if file not found in Assets */
	static inline fs::path FindFile(string _name, ResourceType _type = ResourceType::Unset)
	{
		fs::path quickPath = fs::current_path().concat("\\Assets");

		switch (_type)
		{
		case Resources::ResourceType::Shader:
			quickPath.concat("\\Shaders"); break;

		case Resources::ResourceType::Texture:
			quickPath.concat("\\Textures"); break;

		case Resources::ResourceType::Mesh:
			quickPath.concat("\\Meshes"); break;

		case Resources::ResourceType::Material:
			quickPath.concat("\\Materials"); break;

		case Resources::ResourceType::Sound:
			quickPath.concat("\\Sounds"); break;

		case Resources::ResourceType::Physical:
			quickPath.concat("\\Physicals"); break;

		case Resources::ResourceType::Scene:
			quickPath.concat("\\Scenes"); break;

		case Resources::ResourceType::Script:
			quickPath.concat("\\Scripts"); break;

		case Resources::ResourceType::Unset:
		default: // No quick path
			break;
		}

		for (fs::path file : fs::recursive_directory_iterator(quickPath))
			if (file.filename().string().find(_name) != string::npos)
				return file;
		return fs::path();
	}

	/* Output : Returns contents of a file */
	static inline string ReadFileContent(fs::path _path)
	{
		string content; std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			file.open(_path.string());

			std::stringstream shaderStream;
			shaderStream << file.rdbuf();

			file.close();
			content = shaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			DEBUG_ERROR("SHADER FILE READ FAILLED: ", e.what());
		}
		return content;
	}
}