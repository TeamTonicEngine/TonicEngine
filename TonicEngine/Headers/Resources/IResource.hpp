#pragma once

#include "pch.hpp"

#include <string>
#include <cstring>
#include <fstream>

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
		bool bRead_ = false;
		bool bLoaded_ = false;

		ResourceType type_ = ResourceType::Unset;

	private:
		unsigned resourceId_ = -1;
		fs::path resourcePath_ = fs::path();
		string name_ = "";

	public:
		__declspec(property(get = GetResourceId, put = SetResourceId))
			unsigned ID;
		__declspec(property(get = GetResourcePath, put = SetResourcePath))
			fs::path path;
		__declspec(property(get = GetResourceName, put = SetResourceName))
			string name;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		virtual TONIC_ENGINE_API ~IResource() = default;

		void TONIC_ENGINE_API ReadFileTimed(const string _name);

		virtual void TONIC_ENGINE_API ReadFile(const string _name) = 0;
		virtual void TONIC_ENGINE_API ReadFile(const fs::path _path) = 0;
		virtual void TONIC_ENGINE_API MetaWriteFile(const string _name) = 0;
		virtual void TONIC_ENGINE_API MetaReadFile(const string _name) = 0;

		virtual void TONIC_ENGINE_API Use() const = 0;

		virtual void TONIC_ENGINE_API ResourceUnload() = 0;

		virtual bool TONIC_ENGINE_API IsReadFinished();
		virtual bool TONIC_ENGINE_API IsLoaded();
		virtual void TONIC_ENGINE_API BypassLoad();

	protected:	// Properties get & set (see public vars)
		virtual void TONIC_ENGINE_API SetResourceId(const unsigned& _newId);
	public:
		virtual unsigned TONIC_ENGINE_API GetResourceId() const;

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

	/* Output : Returns empty path if file not found */
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