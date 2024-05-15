#include "Core/EditorWindow.hpp"
#include "Core/Utils.hpp"

Maths::Vec4 Core::Applications::EditorWindow::DrawTree(u64 _id)
{
	bool recurse = true;

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanFullWidth;
	if (!ENGINE.ENT_MNGR->GetEntityChildren(_id).size())
		nodeFlags |= ImGuiTreeNodeFlags_Bullet;
	if (_id == selectedId)
		nodeFlags |= ImGuiTreeNodeFlags_Selected;

	static bool bg = false;
	if (_id == ECS::ROOT_ENTITY_ID)
		bg = false;

	if (_id != ECS::ROOT_ENTITY_ID)
	{
		bg = !bg;

		if (bg)
		{

			float y1 = (ImGui::GetCursorScreenPos().y - 2);// +(23 * (int)row_n);
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(
				ImGui::GetCurrentWindow()->WorkRect.Min.x,
				y1
			), ImVec2(
				ImGui::GetCurrentWindow()->WorkRect.Max.x,
				y1 + 23
			), ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.3f, 0.3f)));

		}

		std::string name = ENGINE.ENT_MNGR->GetEntityData(_id)->name;
		//ImGui::SetNextItemWidth(10);
		//ImGui::PushStyleVar(ImGuiStyleVar_FrameWid, ImVec2(10.f, 50.f));

		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float backup_work_max_x = window->WorkRect.Max.x;
		//window->WorkRect.Max.x = ImGui::GetWindowWidth() + 99;//window->DC.CursorPos.x + ImGui::GetWindowWidth() - 50;
		float maxRect = window->WorkRect.Max.x -= 50;
		recurse = ImGui::TreeNodeEx(name.c_str(), nodeFlags);
		window->WorkRect.Max.x = backup_work_max_x;
		//recurse = ImGui::TreeNodeEx(name.c_str(), nodeFlags);
		//ImGui::PopStyleVar();


		if (ImGui::IsItemClicked())
		{
			selectedId = _id;
		}

		ImGui::SameLine();

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 56 - (window->ScrollbarY * 15));

		ImGui::PushID((int)_id);
		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.5, 0.5, 0.5, 0.5 });
		if (ImGui::ImageButton((ImTextureID)(u64)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\trash.png")->ID,
			{ 19,19 }, { 0,1 }, { 1,0 }, 0))
		{
			selectedId = ECS::ROOT_ENTITY_ID;
			ENGINE.ENT_MNGR->DestroyEntity(_id);
		}

		ImGui::SameLine();
		if (ImGui::ImageButton((ImTextureID)(u64)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\plus.png")->ID,
			{ 19,19 }, { 0,1 }, { 1,0 }, 0))
		{
			//ENGINE.ENT_MNGR->AddNewEntity();
			//ENGINE.ENT_MNGR->ParentEntity(_id, ENGINE.ENT_MNGR->AddNewEntity());
			ENGINE.ENT_MNGR->AddChild(_id, ENGINE.ENT_MNGR->AddNewEntity());
		}

		ImGui::PopStyleColor(2);
		ImGui::PopID();




		//row_n++;
	}

	const ImRect nodeRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	if (recurse)
	{
		const ImColor TreeLineColor = ImGui::GetColorU32(ImGuiCol_Text);
		const float SmallOffsetX = -11.0f; //for now, a hardcoded value; should take into account tree indent size
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 verticalLineStart = ImGui::GetCursorScreenPos();
		verticalLineStart.x += SmallOffsetX; //to nicely line up with the arrow symbol
		ImVec2 verticalLineEnd = verticalLineStart;

		for (const auto& child : ENGINE.ENT_MNGR->GetEntityChildren(_id))
		{
			const float HorizontalTreeLineSize = 8.0f; //chosen arbitrarily

			Maths::Vec4 v4 = DrawTree(child);
			ImRect childRect = { {v4.x,v4.y}, {v4.z, v4.w} };

			const float midpoint = (childRect.Min.y + childRect.Max.y) / 2.0f;
			drawList->AddLine(ImVec2(verticalLineStart.x, midpoint), ImVec2(verticalLineStart.x + HorizontalTreeLineSize, midpoint), TreeLineColor);
			verticalLineEnd.y = midpoint;
		}
		drawList->AddLine(verticalLineStart, verticalLineEnd, TreeLineColor);
		if (_id != ECS::ROOT_ENTITY_ID)
			ImGui::TreePop();
	}
	return { nodeRect.Min.x, nodeRect.Min.y, nodeRect.Max.x, nodeRect.Max.y };
}