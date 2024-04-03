#include "ResourceManager.hpp"

template<class IRes>
IRes* Resources::ResourceManager::Create(string _name, bool _forceReload)
{
	if (!std::is_base_of<IResource, IRes>())
	{
		DEBUG_ERROR("%s is not a IResource, ResourceManager cannot handle it /!\\", _name.c_str());
		return nullptr;
	}

	fs::path path2File = FindFile(_name);

	if (path2File.empty())
	{
		DEBUG_ERROR("%s: file not found", _name.c_str());
		return nullptr;
	}

	if (resources_[_name])
		if (!_forceReload)
			return (IRes*)resources_[_name].get();
		else
			Delete(_name);

	resources_[_name] = std::make_shared<IRes>();

	resources_[_name]->name = _name;
	resources_[_name]->path = path2File;
	resources_[_name]->ReadFile(path2File);

	return (IRes*)resources_[_name].get();
}

template<class IRes>
IRes* Resources::ResourceManager::Get(string _name)
{
	if (resources_[_name])
		return (IRes*)resources_[_name].get();

	DEBUG_WARNING("%s was NOT found in resourceManager", _name.c_str());
	return nullptr;
}