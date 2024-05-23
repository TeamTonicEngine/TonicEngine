#include "Core/EditorWindow.hpp"
#include "Core/Utils.hpp"

#include <shellapi.h>
#include <filesystem>

std::string GetNewSceneName(std::string _path)
{
	std::string newName = "NewScene";
	if (std::filesystem::exists((_path + "\\" + newName + ".ic3")))
	{
		int newCount = 1;
		while (std::filesystem::exists(_path + "\\" + newName + std::to_string(newCount) + ".ic3"))
		{
			newCount++;
		}
		return newName + std::to_string(newCount);
	}

	return newName;
}

void Core::Applications::EditorWindow::DrawContentBrowser()
{
	static auto lastClickTime = std::chrono::high_resolution_clock::now();
	static s32 lastClickedId = 0;
	static bool nameEdit = false;

	if (ImGui::BeginPopup("NewContent"))
	{

		if (ImGui::MenuItem("New Scene")) {
			// Handle button 1 click
			std::string newFileName = GetNewSceneName(static_cast<Resources::Archi*>(p_projectArchi_)->path);
			std::string newFile = static_cast<Resources::Archi*>(p_projectArchi_)->path + '\\' + newFileName + ".ic3";
			Resources::Scene::SaveFile(newFile.c_str());
			ENGINE.RES_MNGR->Create<Resources::Scene>(newFile);

			static_cast<Resources::Archi*>(p_projectArchi_)->file.push_back({ newFileName , Resources::FileExt::ic3,newFile });

			std::cout << "Scene saved at " << static_cast<Resources::Archi*>(p_projectArchi_)->path << std::endl;
		}
		ImGui::EndPopup();
	}
	
	
	
	if (bOpenProjectPopup)
	{
		ImGui::OpenPopup("NewContent");
		bOpenProjectPopup = false;
	}
	

	ImVec2 windowSize = ImGui::GetWindowSize();
	float scroll = ImGui::GetScrollY();

	int temp = (int)std::floor(windowSize.x / 85);

	int x = 0;
	int y = 0;

	if (static_cast<Resources::Archi*>(p_projectArchi_)->parent)
	{
		Resources::Archi* parent = static_cast<Resources::Archi*>(p_projectArchi_)->parent;
		if (ImGui::Button("...") && parent)
			p_projectArchi_ = parent;

		std::string partentPath;
		for (Resources::Archi* subParent = parent; subParent; subParent = subParent->parent)
		{
			partentPath = (subParent->name.size() > 0 ? subParent->name : ".") + "/" + partentPath;
		}

		if (ImGui::IsItemHovered() && parent)
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(partentPath.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		x = 34;
	}

	ImGui::SetCursorPos({ 8.f + x ,30.f });

	std::string curFolName = static_cast<Resources::Archi*>(p_projectArchi_)->name;

	if (curFolName.size() > 0)
	{
		curFolName += " |";
		ImGui::Text(curFolName.c_str());
		x += (int)curFolName.size() * 8;
	}

	ImGui::SetCursorPos({ 8.f + x ,30.f });

	std::string t = "File : ";
	t += std::to_string(static_cast<Resources::Archi*>(p_projectArchi_)->file.size());
	ImGui::Text(t.c_str());

	x = 0;
	int prei = 0;

	if (static_cast<Resources::Archi*>(p_projectArchi_)->subFolder.size() > 0)
	{
		for (int i = 0; i < static_cast<Resources::Archi*>(p_projectArchi_)->subFolder.size(); [&] {
			i++;
				if (!temp || i % temp != 0)
				{
					x += 84;
						prei = i;
				}
				else
				{
					x = 0;
						prei = 0;
						y += 92;
				}
			}())
		{
			ImGui::PushID(i);
			std::string dirName = "##" + static_cast<Resources::Archi*>(p_projectArchi_)->subFolder[i]->name;

			ImGui::SetCursorPos({ 8.f + x ,130.f + y });

			std::string modifiedFldrName, fldrName;
			modifiedFldrName = fldrName = static_cast<Resources::Archi*>(p_projectArchi_)->subFolder[i]->name;

			if (modifiedFldrName.length() > 11)
			{
				modifiedFldrName.erase(8, modifiedFldrName.length() - 8);
				modifiedFldrName += "...";
			}

			bool nameHasChanged = false;

			ImGui::Text(modifiedFldrName.c_str());
			
			if (ImGui::IsItemHovered() && !nameEdit)
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(fldrName.c_str());
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}

			
			ImGui::SetCursorPos({ 8.f + x ,52.f + y });
			
			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.5, 0.5, 0.5, 0.5 });
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\folder.png")->ID), { 76,76 }, { 0,1 }, { 1,0 }, 0))
			{
				auto currentTime = std::chrono::high_resolution_clock::now();
				std::chrono::duration<float> timeSinceLastClick = currentTime - lastClickTime;
				if (lastClickedId == i && timeSinceLastClick.count() < 0.3)
					p_projectArchi_ = static_cast<Resources::Archi*>(p_projectArchi_)->subFolder[i];
				lastClickedId = i;
				lastClickTime = currentTime;
			}
			ImGui::PopStyleColor(2);
			ImGui::PopID();
		}
	}

	for (int i = 0; i < static_cast<Resources::Archi*>(p_projectArchi_)->file.size(); [&] {
		i++;
			if (!temp || (i + prei) % temp != 0)
				x += 84;
			else
			{
				x = 0;
					y += 92;
			}
		}())
	{
		ImGui::SetCursorPos({ 8.f + x ,130.f + y });

		std::string modifiedResName, resName;
		modifiedResName = resName = std::get<0>(static_cast<Resources::Archi*>(p_projectArchi_)->file[i]);

		//Determine the icon from the extension ----------------------------------------------------------------------------------------
		u64 fileTextId = 0;
		switch (std::get<1>(static_cast<Resources::Archi*>(p_projectArchi_)->file[i]))
		{
		case Resources::FileExt::png:
		case Resources::FileExt::jpg:
		case Resources::FileExt::bmp:
			fileTextId = ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\image-file.png")->ID;
			break;
		case Resources::FileExt::frag:
		case Resources::FileExt::vert:
			fileTextId = ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\favourite-file.png")->ID;
			break;
		case Resources::FileExt::obj:
		case Resources::FileExt::fbx:
			fileTextId = ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\3d-file.png")->ID;
			break;
		case Resources::FileExt::ogg:
		case Resources::FileExt::wav:
		case Resources::FileExt::mp3:
			fileTextId = ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\audio-file.png")->ID;
			break;
		case Resources::FileExt::ic3:
			fileTextId = ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\scene-file.png")->ID;
			break;
		default:
			fileTextId = ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\file.png")->ID;
			break;
		}
		//------------------------------------------------------------------------------------------------------------------------------


		//Clamp the name to eleven character -------------------------------------------------------------------------------------------
		if (modifiedResName.length() > 11)
		{
			modifiedResName.erase(8, modifiedResName.length() - 8);
			modifiedResName += "...";
		}
		//------------------------------------------------------------------------------------------------------------------------------


		ImGui::Text(modifiedResName.c_str()); //HERE


		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(resName.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos({ 8.f + x ,52.f + y });
		ImGui::PushID(i);
		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.5, 0.5, 0.5, 0.5 });
		if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(fileTextId), { 76,76 }, { 0,1 }, { 1,0 }, 0))
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> timeSinceLastClick = currentTime - lastClickTime;
			if (lastClickedId == i && timeSinceLastClick.count() < 0.3)
			{
				if (std::get<1>(static_cast<Resources::Archi*>(p_projectArchi_)->file[i]) == Resources::FileExt::ic3)
				{
					selectedId = 0;
					selectedHasTransform = false;
					ENGINE.RES_MNGR->Get<Resources::Scene>(std::get<2>(static_cast<Resources::Archi*>(p_projectArchi_)->file[i]))->Use();
					curentScenePath = std::get<2>(static_cast<Resources::Archi*>(p_projectArchi_)->file[i]);
				}
				else
				{
					auto wtext = stringToWstring(std::get<2>(static_cast<Resources::Archi*>(p_projectArchi_)->file[i]));
					ShellExecuteW(NULL, L"open", wtext.c_str(), NULL, NULL, SW_SHOWDEFAULT);
				}
			}
			lastClickedId = i;
			lastClickTime = currentTime;
		}
		ImGui::PopStyleColor(2);
		ImGui::PopID();
	}
}