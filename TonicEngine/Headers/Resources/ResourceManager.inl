#pragma once
#include "ResourceManager.hpp"


template<class IRes>
std::shared_ptr<IRes> Resources::ResourceManager::Create(fs::path _path, bool _forceReload,bool _bMultithreadIt)
{
	if (!std::is_base_of<IResource, IRes>())
	{
		DEBUG_ERROR("%s is not a IResource, ResourceManager cannot handle it /!\\", _path.filename().string().c_str());
		return nullptr;
	}

	u64 id = GenerateId(_path.string());

	if (resources_[id])
		if (!_forceReload)
			return std::dynamic_pointer_cast<IRes>(resources_[id]);
		else
			Delete(id);

	resources_[id] = std::make_shared<IRes>();

	resources_[id]->name = _path.filename().replace_extension("").string().c_str();
	resources_[id]->path = _path;
	resources_[id]->RMID = id;


	//if (strcmp(typeid(IRes).name(), typeid(Resources::Texture).name()) == 0)

	if (typeid(IRes) == typeid(Resources::Texture))
		textureList.push_back(resources_[id]);
	if (typeid(IRes) == typeid(Resources::Mesh))
		meshList.push_back(resources_[id]);
	if (typeid(IRes) == typeid(Resources::Sound))
		audioList.push_back(resources_[id]);

	if (_bMultithreadIt)
	{
		std::shared_ptr<IRes> newResource = std::dynamic_pointer_cast<IRes>(resources_[id]);
		resourcePool_.AddToQueue(
			[newResource, _path]()
			{ newResource->ReadFileTimed(_path); },
			resources_[id]->name + " threaded loading"
		);
		return newResource;
	}
	else
	{
		resources_[id]->ReadFile(_path);
		resources_[id]->LoadFile();
	}

	return std::dynamic_pointer_cast<IRes>(resources_[id]);
}

template<class IRes>
std::shared_ptr<IRes> Resources::ResourceManager::Get(u64 _id)
{
	if (resources_.find(_id) != resources_.end())
		return std::dynamic_pointer_cast<IRes>(resources_[_id]);
	
	DEBUG_WARNING("%d was NOT found in resourceManager", _id);
	return nullptr;
}

template<class IRes>
std::shared_ptr<IRes> Resources::ResourceManager::Get(fs::path _path)
{
	return Get<IRes>(GenerateId(_path.string()));
}