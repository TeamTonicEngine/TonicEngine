#include "Core/Utils.hpp"

bool DrawVec3Control(const std::string& label, Maths::Vec3& values, float resetValue, bool lock)
{
	bool result = false;
	Maths::Vec3 preValues = values;

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());


	ImGui::PushMultiItemsWidths(3, ImGui::GetWindowWidth() * 0.6f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
	{
		values.x = resetValue;
		result = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	result |= ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		values.y = resetValue;
		result = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	result |= ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
	{
		values.z = resetValue;
		result = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	result |= ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::PopID();

	if (lock)
	{
		Maths::Vec3 diff(preValues - values);
		if (diff.x)
		{
			float ratio = values.x / preValues.x;
			values.y *= ratio;
			values.z *= ratio;
		}
		if (diff.y)
		{
			float ratio = values.y / preValues.y;
			values.x *= ratio;
			values.z *= ratio;
		}
		if (diff.z)
		{
			float ratio = values.z / preValues.z;
			values.x *= ratio;
			values.y *= ratio;
		}
	}

	return result;
}


bool TextureMenuItem(const char* label, ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col, const char* shortcut, bool selected, bool enabled)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	ImGuiStyle& style = g.Style;
	ImVec2 pos = window->DC.CursorPos;
	ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	// We've been using the equivalent of ImGuiSelectableFlags_SetNavIdOnHover on all Selectable() since early Nav system days (commit 43ee5d73),
	// but I am unsure whether this should be kept at all. For now moved it to be an opt-in feature used by menus only.
	ImGuiSelectableFlags flags = ImGuiSelectableFlags_SelectOnRelease | ImGuiSelectableFlags_SetNavIdOnHover | (enabled ? 0 : ImGuiSelectableFlags_Disabled);
	bool pressed = false;
	
	// Menu item inside a vertical menu
	// (In a typical menu window where all items are BeginMenu() or MenuItem() calls, extra_w will always be 0.0f.
	//  Only when they are other items sticking out we're going to add spacing, yet only register minimum width into the layout system.
	float shortcut_w = shortcut ? ImGui::CalcTextSize(shortcut, NULL).x : 0.0f;
	float min_w = window->DC.MenuColumns.DeclColumns(label_size.x, shortcut_w, IM_FLOOR(g.FontSize * 1.20f)); // Feedback for next frame
	float extra_w = ImMax(0.0f, ImGui::GetContentRegionAvail().x - min_w);
		
	//pressed = ImGui::Selectable(label, false, flags | ImGuiSelectableFlags_SpanAvailWidth, ImVec2(min_w, 0.0f));
	pressed = ImGui::Selectable(label, false, flags | ImGuiSelectableFlags_SpanAvailWidth, ImVec2(min_w, label_size.y + size.y +5));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY()-size.y - 6);
	ImGui::Image(user_texture_id, size, uv0, uv1, tint_col, border_col);

	if (shortcut_w > 0.0f)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
		ImGui::RenderText(pos + ImVec2(window->DC.MenuColumns.Pos[1] + extra_w, 0.0f), shortcut, NULL, false);
		ImGui::PopStyleColor();
	}
	if (selected)
		ImGui::RenderCheckMark(window->DrawList, pos + ImVec2(window->DC.MenuColumns.Pos[2] + extra_w + g.FontSize * 0.40f, g.FontSize * 0.134f * 0.5f), ImGui::GetColorU32(enabled ? ImGuiCol_Text : ImGuiCol_TextDisabled), g.FontSize * 0.866f);

	IMGUI_TEST_ENGINE_ITEM_INFO(window->DC.LastItemId, label, window->DC.ItemFlags | ImGuiItemStatusFlags_Checkable | (selected ? ImGuiItemStatusFlags_Checked : 0));
	return pressed;
}



//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Code from imgui repo to use string with input text
// https://github.com/ocornut/imgui/blob/master/misc/cpp/imgui_stdlib.cpp
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
	InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
		std::string* str = user_data->Str;
		IM_ASSERT(data->Buf == str->c_str());
		str->resize(data->BufTextLen);
		data->Buf = (char*)str->c_str();
	}
	else if (user_data->ChainCallback)
	{
		// Forward to user callback, if any
		data->UserData = user_data->ChainCallbackUserData;
		return user_data->ChainCallback(data);
	}
	return 0;
}
bool StringInputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
	IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
	flags |= ImGuiInputTextFlags_CallbackResize;

	InputTextCallback_UserData cb_user_data;
	cb_user_data.Str = str;
	cb_user_data.ChainCallback = callback;
	cb_user_data.ChainCallbackUserData = user_data;
	return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

std::wstring stringToWstring(const std::string& str)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wstr(size - 1, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size - 1);
	return wstr;
}