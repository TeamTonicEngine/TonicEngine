#include "pch.hpp"

#include "Resources/ResourceManager.hpp"
#include "Resources/Resources.hpp"

Resources::FileExt Resources::GetExt(std::string _ext)
{
	_ext = [_ext] {std::string result; for (const char& ch : _ext) { result += std::tolower(ch); } return result; }();

	if (stringToFileExt.find(_ext) != stringToFileExt.end())
		return stringToFileExt[_ext];

	return FileExt::none;
}

void Resources::ResourceManager::RegisterAllFileFrom(const char* _path, Archi* _arch, std::vector<int> _lastLineAt, int _recurrence)
{
	if (_recurrence == 0) // Draw the origin of execution
	{
		std::cout << "------------------------------------------------------------\n";
		std::cout << _path << std::endl;
		std::cout << "------------------------------------------------------------\n";
		std::cout << "    \xe2\x94\x82   \n";
		_arch->path = _path;
	}

	std::string tab = "";
	for (int i = 0; i < _recurrence; i++)
	{
		auto checkIfBlank = [i, _lastLineAt]() {for (const auto& check : _lastLineAt) { if (check == i) { return true; } }return false; };
		if (checkIfBlank())
			tab += "        ";
		else
			tab += "    \xe2\x94\x82   ";
	}

	std::filesystem::path executionPath = _path;

	if (std::filesystem::exists(executionPath) && std::filesystem::is_directory(executionPath))
	{
		std::filesystem::directory_entry lastEntry;
		for (const auto& entry : std::filesystem::directory_iterator(executionPath, std::filesystem::directory_options::skip_permission_denied))
			lastEntry = entry;

		try
		{
			for (const auto& entry : std::filesystem::directory_iterator(executionPath, std::filesystem::directory_options::skip_permission_denied))
			{
				std::string caseTab = "";
				if (entry == lastEntry)
					caseTab = "    \xe2\x94\x94\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80";
				else
					caseTab = "    \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80";

				if (entry.is_directory())
				{
					std::cout << tab << caseTab << "Folder: " << entry.path().filename().string().c_str() << std::endl;

					Archi* nArchi = new Archi(entry.path().filename().string(), entry.path().string(), _arch);
					_arch->subFolder.push_back(nArchi);

					if (entry == lastEntry)
					{
						_lastLineAt.push_back(_recurrence);
						RegisterAllFileFrom(entry.path().string().c_str(), _arch->subFolder.back(), _lastLineAt, _recurrence + 1);
					}
					else
						RegisterAllFileFrom(entry.path().string().c_str(), _arch->subFolder.back(), _lastLineAt, _recurrence + 1);
				}
				else
				{
					const char* color = "";
					bool bregister = true;

					FileExt ext = GetExt(entry.path().filename().extension().string());

					switch (ext)
					{
					case FileExt::png:
					case FileExt::jpg:
					case FileExt::bmp:
						//Create<Texture>(entry.path().string());
						color = "\033[38;2;242;138;180m";
						break;
					case FileExt::frag:
					case FileExt::vert:
						color = "\033[38;2;138;242;200m";
						break;
					case FileExt::obj:
					case FileExt::fbx:
						//Create<Mesh>(entry.path().string());
						color = "\033[38;2;137;167;241m";
						break;
					case FileExt::ogg:
					case FileExt::wav:
					case FileExt::mp3:
						color = "\033[38;2;241;211;137m";
						break;
					case FileExt::ic3:
						Create<Scene>(entry.path());
						color = "\033[38;2;151;194;31m";
						break;
					default:
						bregister = false;
						break;
					}

					std::string rltPath = entry.path().string();
					rltPath.replace(
						rltPath.find(std::filesystem::current_path().string().c_str()),
						std::filesystem::current_path().string().length(),
						".");

					std::cout << tab << caseTab << "File: " << color << entry.path().filename() << "\033[0m" << " ID = " << GenerateId(rltPath) << std::endl;

					if (bregister)
						_arch->file.push_back(std::make_tuple(entry.path().filename().string(), ext, entry.path().string()));
				}
			}
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
	else
		std::cerr << "The specified path is not a valid directory." << std::endl;
}

Resources::ResourceManager::ResourceManager() { p_files = new Archi(); }

Resources::ResourceManager::~ResourceManager()
{
	resourcePool_.Destroy();

	for (auto IRes : resources_)
		IRes.second->Destroy();

	resources_.clear();
	Font::s_p_defaultFont.reset();
	delete p_files;
}

const bool Resources::ResourceManager::Init()
{
	RegisterAllFileFrom(std::filesystem::current_path().string().c_str(), p_files);

	if (resourcePool_.Init())
		DEBUG_SUCCESS("INITIALIZED RESOURCE MANAGER")
	else
		throw ("ResourceManager failled to initialised");

	return true;
}

u64 Resources::ResourceManager::GenerateId(const std::string& str)
{
	const u64 fnv_prime = 1099511628211ull;
	u64 hash = 14695981039346656037ull; // FNV offset basis

	for (char c : str)
	{
		hash ^= static_cast<u64>(c);
		hash *= fnv_prime;
	}

	return hash;
}

void Resources::ResourceManager::Reload(u64 _id)
{
	if (!resources_[_id])
	{
		DEBUG_ERROR("Resources::ResourceManager::Reload() Cannot reload a resource that doesn't exist ! (%d)", _id);
		return;
	}

	switch (resources_[_id].get()->type_)
	{
	case ResourceType::Material:
		Create<Material>(resources_[_id]->name, true);
		break;
	case ResourceType::Mesh:
		Create<Mesh>(resources_[_id]->name, true);
		break;
	case ResourceType::Font:
		Create<Font>(resources_[_id]->name, true);
		break;
	case ResourceType::Scene:
		Create<Scene>(resources_[_id]->name, true);
		break;
	case ResourceType::Shader:
		Create<Shader>(resources_[_id]->name, true);
		break;
	case ResourceType::Sound:
		Create<Sound>(resources_[_id]->name, true);
		break;
	case ResourceType::Texture:
		Create<Texture>(resources_[_id]->name, true);
		break;

	case ResourceType::Unset:
	default:
		DEBUG_ERROR("Resources::ResourceManager::Reload() Resource UNSET for %d", _id);
		break;
	}
}

void Resources::ResourceManager::Delete(u64 _id)
{
	if (resources_.find(_id) != resources_.end())
	{
		resources_[_id]->Destroy();
		resources_.erase(_id);

		DEBUG_LOG("%d has been delete from ResourceManager", _id);
		return;
	}

	DEBUG_WARNING("%d was NOT deleted from resourceManager, item NOT found", _id);
}