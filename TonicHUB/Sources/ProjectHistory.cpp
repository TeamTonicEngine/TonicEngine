#include "ProjectHistory.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "Images.hpp"

ProjectHistory::~ProjectHistory()
{
    data.clear();
}

void ProjectHistory::PlaceOnTop(std::string _projectPath)
{
    FileData newData;
    for (int i = 0; i < data.size(); i++)
    {
        if (_projectPath == data[i].projectPath)
        {
            newData = data[i];
            data.erase(data.begin() + i);
        }
    }

    data.insert(data.begin(), newData);
}
void ProjectHistory::Remove(std::string _projectPath)
{
    FileData newData;
    for (int i = 0; i < data.size(); i++)
    {
        if (_projectPath == data[i].projectPath)
        {
            newData = data[i];
            data.erase(data.begin() + i);
        }
    }
}

void ProjectHistory::Open()
{
    std::ifstream file("projectHistory.txt");
    if (file.is_open()) {
        std::string line;

        while (std::getline(file, line)) {
            std::cout << line << std::endl;
            ProjectFile* projectFile = OpenProject(line);
            if (projectFile)
            {
                FileData newData =
                {
                    line,
                    projectFile->staticData.sizeOfPreview > 0 ? LoadImageFromHardData(projectFile->staticData.previewWidth, projectFile->staticData.previewHeight, projectFile->staticData.previewChannels, projectFile->preview) : GetNoImageId(),
                    projectFile->name
                };
                
                data.push_back(newData);

                delete[] projectFile->name;
                delete[] projectFile->description;
                delete[] projectFile->preview;
                delete projectFile;
            }
        }

        file.close();
    }
}
void ProjectHistory::Save()
{
    std::ofstream  file("projectHistory.txt");
    if (file.is_open()) {
        std::string line;

        for (const FileData& line : data)
        {
            file << line.projectPath << std::endl;

        }

        file.close();
    }
}

template<typename T>
T convertEndian(T value) {
    static_assert(std::is_integral<T>::value, "Type must be integral");
    constexpr size_t size = sizeof(T);
    union {
        T value;
        unsigned char bytes[size];
    } result;
    result.value = value;
    for (size_t i = 0; i < size / 2; i++) {
        std::swap(result.bytes[i], result.bytes[size - i - 1]);
    }
    return result.value;
}

void ProjectHistory::CreateProject(std::string _projectPath, std::string _projectName, int _prevWidth, int _prevHeight, unsigned char* _prevData, int _prevChannels, uint32_t _textureID)
{
    _projectPath = _projectPath + '\\' + _projectName;

    std::filesystem::create_directories(_projectPath);

    _projectPath = _projectPath + '\\' + _projectName + ".tonic";

    std::ofstream  file(_projectPath, std::ios::binary);
    if (file.is_open()) {
        ProjectFile* projectFile = new ProjectFile(
            {
                {
                    FILE_VERSION,
                    ENGINE_VERSION,
                    static_cast<uint16_t>(_projectName.length()),
                    0,
                    static_cast<uint16_t>(_prevWidth),
                    static_cast<uint16_t>(_prevHeight),
                    static_cast<uint16_t>(_prevChannels),
                    static_cast<uint32_t>(_prevWidth * _prevHeight * _prevChannels)
            },
            nullptr,
            nullptr,
            reinterpret_cast<char*>(_prevData)
        });

        //file.read(reinterpret_cast<char*>(&value), sizeof(value));
        file.write(reinterpret_cast<char*>(&projectFile->staticData), sizeof(projectFile->staticData));
        file.write(_projectName.c_str(), _projectName.length());
        //file.write(projectFile->description, projectFile->staticData.descLength);
        file.write(projectFile->preview, projectFile->staticData.sizeOfPreview);

        file.close();

        FileData newData = { _projectPath, _textureID, _projectName };

        data.insert(data.begin(), newData);
    }
}

void ProjectHistory::ImportProject(std::string _projectPath)
{
    for (int i = 0; i < data.size(); i++)
    {
        if (_projectPath == data[i].projectPath)
        {
            FileData copy = data[i];
            data.erase(data.begin() + i);
            data.insert(data.begin(), copy);
            return;
        }
    }

    ProjectFile* newFile = OpenProject(_projectPath);

    if (newFile->staticData.engineMinVersion < ENGINE_VERSION || newFile->staticData.fileVersion > FILE_VERSION )
    {
        delete[] newFile->name;
        delete[] newFile->description;
        delete[] newFile->preview ;
        delete newFile;
        return;
    }

    FileData temp =
    {
        _projectPath,
        newFile->staticData.sizeOfPreview > 0 ? LoadImageFromHardData(newFile->staticData.previewWidth, newFile->staticData.previewHeight, newFile->staticData.previewChannels, newFile->preview) : GetNoImageId(),
        newFile->name
    };

    data.insert(data.begin(), temp);
    delete[] newFile->name;
    delete[] newFile->description;
    delete[] newFile->preview;
    delete newFile;
}

ProjectFile* ProjectHistory::OpenProject(std::string _projectPath)
{
    std::ifstream  file(_projectPath, std::ios::binary);
    if (file.is_open()) {
        ProjectFile* data = new ProjectFile;
        file.read(reinterpret_cast<char*>(&data->staticData), sizeof(data->staticData));

        data->name = new char[data->staticData.nameLenght + 1];
        file.read(data->name, data->staticData.nameLenght);
        data->name[data->staticData.nameLenght] = '\0';

        data->description = new char[data->staticData.descLength + 1];
        file.read(data->description, data->staticData.descLength);
        data->description[data->staticData.descLength] = '\0';

        data->preview = new char[data->staticData.sizeOfPreview + 1];
        file.read(data->preview, data->staticData.sizeOfPreview);
        data->preview[data->staticData.sizeOfPreview] = '\0';

        file.close();
        return data;
    }
    return nullptr;
}

std::vector<FileData> ProjectHistory::GetFiles(std::string _filter)
{
    if (!_filter.size())
        return data;

    std::vector<FileData> result;

    for (const FileData& file : data)
    {
        bool hasFilter = true;
        if (_filter.size() > file.name.length())
            continue;
        for (int i = 0; i < _filter.size(); i++)
        {
            char filterChar = std::tolower(_filter[i]);
            char nameChar = std::tolower(file.name[i]);
            if (filterChar != nameChar)
            {
                hasFilter = false;
                break;
            }
        }
        if(hasFilter)
            result.insert(result.begin(), file);
    }
    return result;
}