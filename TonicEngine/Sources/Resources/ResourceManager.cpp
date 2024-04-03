#include "pch.hpp"

#include "Resources\ResourceManager.hpp"

Resources::FileExt Resources::GetExt(std::string _ext)
{
    _ext = [_ext] {std::string result; for (const char& ch : _ext) { result += std::tolower(ch); } return result; }();

    if (stringToFileExt.find(_ext) != stringToFileExt.end()) {
        return stringToFileExt[_ext];
    }

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

    if (std::filesystem::exists(executionPath) && std::filesystem::is_directory(executionPath)) {
        std::filesystem::directory_entry lastEntry;
        for (const auto& entry : std::filesystem::directory_iterator(executionPath, std::filesystem::directory_options::skip_permission_denied)) {
            lastEntry = entry;
        }
        try {
            for (const auto& entry : std::filesystem::directory_iterator(executionPath, std::filesystem::directory_options::skip_permission_denied)) {
                std::string caseTab = "";
                if (entry == lastEntry)
                    caseTab = "    \xe2\x94\x94\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80";
                else
                    caseTab = "    \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80";

                if (entry.is_directory()) {
                    std::cout << tab << caseTab << "Folder: " << entry.path().filename().string().c_str() << std::endl;

                    Archi* nArchi = new Archi(entry.path().filename().string(), _arch);
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

                    switch (GetExt(entry.path().filename().extension().string()))
                    {
                    case FileExt::png:
                    case FileExt::jpg:
                    case FileExt::bmp:
                        color = "\033[38;2;242;138;180m";
                        break;
                    case FileExt::frag:
                    case FileExt::vert:
                        color = "\033[38;2;138;242;200m";
                        break;
                    case FileExt::obj:
                    case FileExt::fbx:
                        color = "\033[38;2;137;167;241m";
                        break;
                    case FileExt::ogg:
                    case FileExt::mp3:
                        color = "\033[38;2;241;211;137m";
                        break;
                    default:
                        bregister = false;
                        break;
                    }

                    std::cout << tab << caseTab << "File: " << color << entry.path().filename() << "\033[0m" << std::endl;

                    if (bregister)
                        _arch->file.push_back(entry.path().filename().string());
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    else {
        std::cerr << "The specified path is not a valid directory." << std::endl;
    }
}

Resources::ResourceManager::ResourceManager()
{
	p_files = new Archi();
}

Resources::ResourceManager::~ResourceManager()
{
	resourcePool_.Destroy();

	for (auto it = resources_.begin(); it != resources_.end();)
		it = resources_.erase(it);
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

void Resources::ResourceManager::Reload(string _name)
{
	if (!resources_[_name])
	{
		DEBUG_ERROR("Resources::ResourceManager::Reload() Cannot reload a resource that doesn't exist ! (%s)", _name.c_str());
		return;
	}

	switch (resources_[_name].get()->type_)
	{
	case ResourceType::Material:
		//Create<Material>(_name, true);
		DEBUG_ERROR("Resources::ResourceManager::Reload() Resource type not handled for %s", _name.c_str());
		break;
	case ResourceType::Mesh:
		//Create<Mesh>(_name, true);
		DEBUG_ERROR("Resources::ResourceManager::Reload() Resource type not handled for %s", _name.c_str());
		break;
	case ResourceType::Physical:
		//Create<Physical>(_name, true);
		DEBUG_ERROR("Resources::ResourceManager::Reload() Resource type not handled for %s", _name.c_str());
		break;
	case ResourceType::Scene:
		//Create<Scene>(_name, true);
		DEBUG_ERROR("Resources::ResourceManager::Reload() Resource type not handled for %s", _name.c_str());
		break;
	case ResourceType::Script:
		//Create<Script>(_name, true);
		DEBUG_ERROR("Resources::ResourceManager::Reload() Resource type not handled for %s", _name.c_str());
		break;
	case ResourceType::Shader:
		Create<Shader>(_name, true);
		break;
	case ResourceType::Sound:
		//Create<Sound>(_name, true);
		DEBUG_ERROR("Resources::ResourceManager::Reload() Resource type not handled for %s", _name.c_str());
		break;
	case ResourceType::Texture:
		//Create<Texture>(_name, true);
		DEBUG_ERROR("Resources::ResourceManager::Reload() Resource type not handled for %s", _name.c_str());
		break;
	case ResourceType::Unset:
	default:
		DEBUG_ERROR("Resources::ResourceManager::Reload() Resource type not handled for %s", _name.c_str());
		break;
	}
}

void Resources::ResourceManager::Delete(string _name)
{
	if (resources_[_name])
	{
		resources_.erase(_name);
		DEBUG_LOG("%s has been delete from ResourceManager", _name.c_str());
		return;
	}

	DEBUG_WARNING("%s was NOT deleted from resourceManager, item NOT found", _name.c_str());
}