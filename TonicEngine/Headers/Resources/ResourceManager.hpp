#pragma once

#include "Resources/IResource.hpp"

#include <Core/ThreadPool.hpp>

#include <map>
#include <tuple>

namespace Resources
{
	using std::string;

	enum class FileExt
	{
		none,
		//IMAGE
		png,
		jpg,
		bmp,
		//SHADERS
		frag,
		vert,
		//MODELS
		obj,
		fbx,
		//SOUND
		ogg,
		mp3,
		wav,
		//SCENE
		ic3
	};

	struct Archi
	{
		string name;
		std::vector<std::tuple<string, FileExt, string>>file;

		std::vector<Archi*> subFolder;
		Archi* parent;

		inline Archi() : name(""), file({}), subFolder({}), parent(nullptr) {};
		inline Archi(string _name, Archi* _parent = nullptr) : name(_name), file({}), subFolder({}), parent(_parent) {};

		inline ~Archi()
		{
			for (Archi* sub : subFolder)
				delete sub;
		};
	};

	static std::unordered_map<std::string, FileExt> stringToFileExt =
	{
		//IMAGE
		{".png", FileExt::png},
		{".jpg", FileExt::jpg},
		{".bmp", FileExt::bmp},
		//SHADERS
		{".frag", FileExt::frag},
		{".vert", FileExt::vert},
		//MODELS
		{".obj", FileExt::obj},
		{".fbx", FileExt::fbx},
		//SOUND
		{".ogg", FileExt::ogg},
		{".mp3", FileExt::mp3},
		{".wav", FileExt::wav},
		//SCENE
		{".ic3", FileExt::ic3}
	};
	FileExt GetExt(std::string _ext);

	class ResourceManager
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		std::map<u64, std::shared_ptr<IResource>> resources_;
		Core::Threads::ThreadPool resourcePool_;
	public:
		std::vector<std::shared_ptr<IResource>> textureList;
		std::vector<std::shared_ptr<IResource>> meshList;
		std::vector<std::shared_ptr<IResource>> audioList;

		Archi* p_files;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	private:
		void TONIC_ENGINE_API RegisterAllFileFrom(const char* _path, Archi* _arch, std::vector<int> _lastLineAt = {}, int _recurrence = 0);

	public:
		TONIC_ENGINE_API ResourceManager();
		TONIC_ENGINE_API ~ResourceManager();

		const bool TONIC_ENGINE_API Init();

		/* Create Resource:
		Adds a resource into the ResourceManager,
		will not compile if <type> is not a IResource child class
		 - Output: Returns the resource if created successfully or already exist, else returns nullptr */
		template<class IRes>
		std::shared_ptr<IRes> Create(fs::path _path, bool _forceReload = false, bool _bMultithreadIt = true);

		/* - Output: Returns the resource if it exist, else returns nullptr */
		template<class IRes>
		std::shared_ptr<IRes> Get(u64 _id);

		template<class IRes>
		std::shared_ptr<IRes> Get(fs::path _path);

	private:
		/**
		 * Generates an id for the given string using the FNV-1a hashing algorithm.
		 *
		 * @param str the input string to generate the hash for
		 *
		 * @return the id generated for the input string
		 **/
		u64 TONIC_ENGINE_API GenerateId(const std::string& str);

	public:
		/* Reloads the resource if it exist */
		void TONIC_ENGINE_API Reload(u64 _id);
		inline void TONIC_ENGINE_API Reload(fs::path _path) { Reload(GenerateId(_path.string())); }

		/* Deletes the resource if it is found */
		void TONIC_ENGINE_API Delete(u64 _id);
		inline void TONIC_ENGINE_API Delete(fs::path _path) { Delete(GenerateId(_path.string())); }
	};
}

#include "ResourceManager.inl"