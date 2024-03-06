#pragma once

#include "pch.hpp"

#include <string>
#include <cstring>
#include <fstream>

#include <Core/Log.hpp>

namespace Resources
{
	using std::string;

	class TONIC_ENGINE_API IResource
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		bool bRead_ = false;
		bool bLoaded_ = false;

		u32 resourceId_ = -1;
		string resourcePath_ = "";

		enum class ResourceType
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
		ResourceType type = ResourceType::Unset;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		virtual ~IResource() = default;

		void ReadFileTimed(const string _name)
		{
			using namespace std::chrono;
			system_clock::time_point start = system_clock::now();
			ReadFile(_name);
			DEBUG_LOG("%s resource loadtime : %f\n", _name.c_str(),
				(duration<float>(system_clock::now() - start).count()));
		}

		virtual void ReadFile(const string _name) = 0;
		virtual void MetaWriteFile(const string _name) = 0;
		virtual void MetaReadFile(const string _name) = 0;

		virtual void ResourceUnload() = 0;

		inline virtual bool IsReadFinished() { return (bRead_ && !bLoaded_); }
		inline virtual bool IsLoaded() { return bLoaded_; }
		inline virtual void BypassLoad()
		{
			bLoaded_ = true;
			DEBUG_WARNING("Resource loading bypassed for %s ID: %i\n", resourcePath_, resourceId_)
		}

		inline virtual unsigned int GetResourceId() const
		{
			if (resourceId_ == static_cast<unsigned int>(-1))
				DEBUG_WARNING("Could not find id for %s\n", resourcePath_.c_str());
			return resourceId_;
		}

		inline virtual void SetResourcePath(const string& _path) { resourcePath_ = _path; }
		inline virtual string GetResourcePath() const
		{
			if (resourcePath_.empty())
				DEBUG_WARNING("Could not find path for %i\n", resourceId_);
			return resourcePath_;
		}
	};

	namespace fs = std::filesystem;
	static inline fs::path FindFile(string _name)
	{
		for (fs::path file : fs::recursive_directory_iterator(fs::current_path()))
			if (file.filename().string().find(_name) != string::npos)
				return file;
		return fs::path();
	}

	//static inline fs::path FindFileInAssets(string _name) // TODO (or improve FindFile())
	//{}
}
