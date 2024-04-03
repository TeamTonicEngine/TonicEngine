#pragma once
#include <vector>
#include <string>

#define FILE_VERSION 1
#define ENGINE_VERSION 1

struct ProjectFile
{
	struct {
		uint16_t fileVersion;
		uint16_t engineMinVersion;
		uint16_t nameLenght;
		uint16_t descLength;
		uint16_t previewWidth;
		uint16_t previewHeight;
		uint16_t previewChannels;
		uint32_t sizeOfPreview;
	} staticData;
	char* name;
	char* description; 
	char* preview;
};

struct FileData
{
	std::string projectPath;
	uint32_t TextureId;
	std::string name;
};

class ProjectHistory
{
private:
	std::vector<FileData> data;
	ProjectFile* OpenProject(std::string _projectPath);
public:
	~ProjectHistory();

	void PlaceOnTop(std::string _projectPath); //called when a project is added or opened.
	void Remove(std::string _projectPath);
	void Open();
	void Save();

	void CreateProject(std::string _projectPath, std::string _projectName, int _prevWidth = 0, int _prevHeight = 0, unsigned char* _prevData = nullptr, int _prevChannels = 0, uint32_t _textureID = 1);
	void ImportProject(std::string _projectPath);

	std::vector<FileData> GetFiles(std::string _filter = "");
};