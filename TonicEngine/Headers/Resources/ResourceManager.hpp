#pragma once

#include "Resources/IResource.hpp"

#include <Core/ThreadPool.hpp>

#include <map>

namespace Resources
{
	using std::string;

	struct Archi
	{
		string name;
		std::vector<string>file;

		std::vector<Archi*> subFolder;
		Archi* parent;

		inline Archi() : name(""), file({}), subFolder({}), parent(nullptr) {};
		inline Archi(string _name, Archi* _parent = nullptr) : name(_name), file({}), subFolder({}), parent(_parent) {};

		inline ~Archi()
		{
			for (Archi* sub : subFolder)
			{
				delete sub;
			}
		};
	};
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
		wav
	};
	static std::map<std::string, FileExt> stringToFileExt =
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
		{".wav", FileExt::wav}
	};

	FileExt GetExt(std::string _ext);

	class ResourceManager
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		std::map<string, std::shared_ptr<IResource>> resources_;
	public:
		Archi* p_files;
		Core::Threads::ThreadPool resourcePool_;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	private:
		void RegisterAllFileFrom(const char* _path, Archi* _arch, std::vector<int> _lastLineAt = {}, int _recurrence = 0);
	public:
		ResourceManager();
		~ResourceManager();

		const bool Init();

		/* Create Resource:
		Adds a resource into the ResourceManager,
		will not compile if <type> is not a IResource child class
		 - Output: Returns the resource if created successfully or already exist, else returns nullptr */
		template<class IRes>
		IRes* Create(string _name, bool _forceReload = false)
			/* Function body is in this hpp file because of template */;

		/* - Output: Returns the resource if it exist, else returns nullptr */
		template<class IRes>
		IRes* Get(string _name)
			/* Function body is in this hpp file because of template */;

		/* Reloads the resource if it exist */
		void Reload(string _name);

		/* Deletes the resource if it is found */
		void Delete(string _name);
	};
}

#include "ResourceManager.inl"