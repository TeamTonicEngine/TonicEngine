#include "pch.hpp"

#include "Resources/IResource.hpp"

void Resources::IResource::ReadFileTimed(const string _name)
{
	using namespace std::chrono;
	system_clock::time_point start = system_clock::now();
	ReadFile(_name);
	DEBUG_LOG("%s resource loadtime : %f", _name.c_str(),
		(duration<float>(system_clock::now() - start).count()));
}

bool Resources::IResource::IsReadFinished() { return (bRead_ && !bLoaded_); }
bool Resources::IResource::IsLoaded() { return bLoaded_; }

void Resources::IResource::BypassLoad()
{
	bLoaded_ = true;
	DEBUG_WARNING("Resource loading bypassed for %s ID: %i", resourcePath_.string().c_str(), resourceId_)
}

void Resources::IResource::SetResourceId(const unsigned& _newId) { resourceId_ = _newId; }

unsigned Resources::IResource::GetResourceId() const
{
	if (resourceId_ == static_cast<unsigned>(-1))
		DEBUG_WARNING("IResource ID is %i, this may or may not be important", resourceId_);
	return resourceId_;
}

void Resources::IResource::SetResourcePath(const fs::path& _newPath) { resourcePath_ = _newPath; }

fs::path Resources::IResource::GetResourcePath() const
{
	if (resourcePath_.empty())
		DEBUG_WARNING("Could not find path for %i", resourceId_);
	return resourcePath_;
}

void Resources::IResource::SetResourceName(const string& _newName)
{
	if (_newName.empty())
		DEBUG_WARNING("New IResource name is empty for #%i", resourceId_);
	name_ = _newName;
}

std::string Resources::IResource::GetResourceName() const { return name_; }