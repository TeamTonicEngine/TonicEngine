#pragma once
#include <string>
#include "Core/Engine.hpp"

bool DrawVec3Control(const std::string& label, Maths::Vec3& values, float resetValue = 0.0f, bool lock = false);

bool TextureMenuItem(const char* label, ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0), const char* shortcut = NULL, bool selected = false, bool enabled = true);

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Code from imgui repo to use string with input text
// https://github.com/ocornut/imgui/blob/master/misc/cpp/imgui_stdlib.cpp
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct InputTextCallback_UserData
{
	std::string* Str;
	ImGuiInputTextCallback  ChainCallback;
	void* ChainCallbackUserData;
};
static int InputTextCallback(ImGuiInputTextCallbackData* data);
bool StringInputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

std::wstring stringToWstring(const std::string& str);