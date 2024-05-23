#pragma once

#include "Core/EditorWindow.hpp"
#include "Core/Utils.hpp"

#include <ECS/Components.hpp>

template<typename Component>
bool CreateNode(u64 _id, const char* name)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float y1 = (ImGui::GetCursorScreenPos().y - 2);
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(
		window->WorkRect.Min.x,
		y1
	), ImVec2(
		window->WorkRect.Max.x,
		y1 + 23
	), ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.3f, 0.3f)));

	float backup_work_max_x = window->WorkRect.Max.x;
	window->WorkRect.Max.x -= 25;
	bool open = ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding);
	window->WorkRect.Max.x = backup_work_max_x;

	ImGui::SameLine();

	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 30 - (window->ScrollbarY * 15));

	ImGui::PushID((int)_id);
	ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.5, 0.5, 0.5, 0.5 });
	if (ImGui::ImageButton((ImTextureID)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\trash.png")->ID,
		{ 19,19 }, { 0,1 }, { 1,0 }, 0))
	{
		ENGINE.ENT_MNGR->RemoveComponent<Component>(_id);
		ImGui::PopStyleColor(2);
		ImGui::PopID();
		if (open)
			ImGui::TreePop();
		return false;
	}
	ImGui::PopStyleColor(2);
	ImGui::PopID();

	return open;
}

//TODO move into a header
struct Icons
{
	ImTextureID trash;
	ImTextureID plus;
	ImTextureID lock;
	ImTextureID unlock;
};

//TODO move into a header ?
void DrawTransformProperty(u64 _id, const Icons& _icons);
void DrawMeshRendererProperty(u64 _id, const Icons& _icons);
void DrawTextRendererProperty(u64 _id, const Icons& _icons);
void DrawDirectionalLightProperty(u64 _id, const Icons& _icons);
void DrawPointLightProperty(u64 _id, const Icons& _icons);
void DrawSpotLightProperty(u64 _id, const Icons& _icons);
void DrawCameraProperty(u64 _id, const Icons& _icons);
void DrawAudioSourceProperty(u64 _id, const Icons& _icons);
void DrawAudioListenerProperty(u64 _id, const Icons& _icons);
void DrawBoxRigidbodyProperty(u64 _id, const Icons& _icons);
void DrawSphereRigidbodyProperty(u64 _id, const Icons& _icons);
void DrawCapsuleRigidbodyProperty(u64 _id, const Icons& _icons);
void DrawScriptComponentProperty(u64 _id, const Icons& _icons);

/////////////////////////////////////////////////////////////////////
void Core::Applications::EditorWindow::DrawProperty(u64 _id)
{
	using namespace ECS::Components;

	if (_id == ECS::ROOT_ENTITY_ID)
		return;

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10,5 });
	std::string name = ENGINE.ENT_MNGR->GetEntityData(_id)->name;

	ImGui::SetNextItemWidth(ImGui::CalcTextSize(name.c_str()).x + 20);
	if (StringInputText("##Name", &name))
		ENGINE.ENT_MNGR->GetEntityData(_id)->name = name;

	ImGui::PopStyleVar(2);

	selectedHasTransform = ENGINE.ENT_MNGR->HasComponent<TransformComponent>(_id);
	bool hasMeshRenderer = ENGINE.ENT_MNGR->HasComponent<MeshRendererComponent>(_id);
	bool hasTextRenderer = ENGINE.ENT_MNGR->HasComponent<TextRendererComponent>(_id);
	bool hasDirectionalLight = ENGINE.ENT_MNGR->HasComponent<DirectionalLightComponent>(_id);
	bool hasPointLight = ENGINE.ENT_MNGR->HasComponent<PointLightComponent>(_id);
	bool hasSpotLight = ENGINE.ENT_MNGR->HasComponent<SpotLightComponent>(_id);
	bool hasCamera = ENGINE.ENT_MNGR->HasComponent<CameraComponent>(_id);
	bool hasScript = ENGINE.ENT_MNGR->HasComponent<CppScriptComponent>(_id);
	bool hasAudioSource = ENGINE.ENT_MNGR->HasComponent<AudioSourceComponent>(_id);
	bool hasAudioListener = ENGINE.ENT_MNGR->HasComponent<AudioListenerComponent>(_id);
	bool hasBoxRigidbody = ENGINE.ENT_MNGR->HasComponent<BoxRigidbodyComponent>(_id);
	bool hasSphereRigidbody = ENGINE.ENT_MNGR->HasComponent<SphereRigidbodyComponent>(_id);
	bool hasCapsuleRigidbody = ENGINE.ENT_MNGR->HasComponent<CapsuleRigidbodyComponent>(_id);

	Icons icons =
	{
		(ImTextureID)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\trash.png")->ID,
		(ImTextureID)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\plus.png")->ID,
		(ImTextureID)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\lock.png")->ID,
		(ImTextureID)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\unlock.png")->ID
	};

	if (selectedHasTransform)
	{
		auto& transformComp = ENGINE.ENT_MNGR->GetComponent<TransformComponent>(_id);
		auto& model = transformComp.GetModel();
		if (preSelectId != selectedId)
		{
			selectedTransform = Maths::Mat4::Translate(transformComp.position);
			switch (gizmod)
			{
			case GizmoMod::ROTATE:
			{
				auto rot = transformComp.rotation;
				selectedTransform *= Maths::Mat4(rot.ToRotationMatrix());
				break;
			}
			case GizmoMod::SCALE:
				selectedTransform *= Maths::Mat4::Scale(transformComp.scale);
				break;
			default:
				break;
			}
			preSelectId = selectedId;
		}
		else
		{
			if (selectedTransform != model)
			{
				if (gizmoUsed)
				{
					switch (gizmod)
					{
					case GizmoMod::TRANSLATE:
						transformComp.SetPosition(Maths::Vec3(selectedTransform.data[12], selectedTransform.data[13], selectedTransform.data[14]));
						break;
					case GizmoMod::ROTATE:
						transformComp.SetRotation(Maths::Quaternions::FromMatrix(selectedTransform).GetNormalized());
						break;
					case GizmoMod::SCALE:
						transformComp.SetScale(Maths::Vec3(selectedTransform[0].GetMagnitude(), selectedTransform[1].GetMagnitude(), selectedTransform[2].GetMagnitude()));
						break;
					default:
						break;
					}
				}
				else
				{
					selectedTransform = Maths::Mat4::Translate(transformComp.position);
					switch (gizmod)
					{
					case GizmoMod::ROTATE:
					{
						auto rot = transformComp.rotation;
						selectedTransform *= Maths::Mat4(rot.ToRotationMatrix());
						break;
					}
					case GizmoMod::SCALE:
						selectedTransform *= Maths::Mat4::Scale(transformComp.scale);
						break;
					default:
						break;
					}
				}
			}
		}

		DrawTransformProperty(_id, icons);
	}

	if (hasMeshRenderer)
		DrawMeshRendererProperty(_id, icons);

	if (hasTextRenderer)
		DrawTextRendererProperty(_id, icons);

	if (hasDirectionalLight)
		DrawDirectionalLightProperty(_id, icons);

	if (hasPointLight)
		DrawPointLightProperty(_id, icons);

	if (hasSpotLight)
		DrawSpotLightProperty(_id, icons);

	if (hasCamera)
		DrawCameraProperty(_id, icons);

	if (hasScript)
		DrawScriptComponentProperty(_id, icons);

	if (hasAudioSource)
		DrawAudioSourceProperty(_id, icons);

	if (hasAudioListener)
		DrawAudioListenerProperty(_id, icons);

	if (hasBoxRigidbody)
		DrawBoxRigidbodyProperty(_id, icons);

	if (hasSphereRigidbody)
		DrawSphereRigidbodyProperty(_id, icons);

	if (hasCapsuleRigidbody)
		DrawCapsuleRigidbodyProperty(_id, icons);

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100);

	if (ImGui::ImageButton(icons.plus, { 21,21 }, { 0,1 }, { 1,0 }, 6))
		ImGui::OpenPopup("newComponentList");

	if (ImGui::BeginPopup("newComponentList", NULL))
	{
		if (!selectedHasTransform && ImGui::MenuItem("Transform##new"))
			ENGINE.ENT_MNGR->AddComponent<TransformComponent>(_id);

		if (!hasMeshRenderer && ImGui::MenuItem("MeshRenderer##new"))
			ENGINE.ENT_MNGR->AddComponent<MeshRendererComponent>(_id);

		if (!hasTextRenderer && ImGui::MenuItem("TextRenderer##new"))
			ENGINE.ENT_MNGR->AddComponent<TextRendererComponent>(_id);

		if (!hasDirectionalLight && ImGui::MenuItem("DirectionalLight##new"))
			ENGINE.ENT_MNGR->AddComponent<DirectionalLightComponent>(_id);

		if (!hasPointLight && ImGui::MenuItem("PointLight##new"))
			ENGINE.ENT_MNGR->AddComponent<PointLightComponent>(_id);

		if (!hasSpotLight && ImGui::MenuItem("SpotLight##new"))
			ENGINE.ENT_MNGR->AddComponent<SpotLightComponent>(_id);

		if (!hasCamera && ImGui::MenuItem("Camera##new"))
			ENGINE.ENT_MNGR->AddComponent<CameraComponent>(_id);

		if (!hasScript && ImGui::MenuItem("Script##new"))
			ENGINE.ENT_MNGR->AddComponent<CppScriptComponent>(_id);

		if (!hasAudioSource && ImGui::MenuItem("AudioSource##new"))
			ENGINE.ENT_MNGR->AddComponent<AudioSourceComponent>(_id);

		if (!hasAudioListener && ImGui::MenuItem("AudioListener##new"))
			ENGINE.ENT_MNGR->AddComponent<AudioListenerComponent>(_id).Init();

		if (!hasBoxRigidbody && !hasSphereRigidbody && !hasCapsuleRigidbody && ImGui::MenuItem("BoxRigidbody##new"))
			ENGINE.ENT_MNGR->AddComponent<BoxRigidbodyComponent>(_id);

		if (!hasBoxRigidbody && !hasSphereRigidbody && !hasCapsuleRigidbody && ImGui::MenuItem("SphereRigidbody##new"))
			ENGINE.ENT_MNGR->AddComponent<SphereRigidbodyComponent>(_id);

		if (!hasBoxRigidbody && !hasSphereRigidbody && !hasCapsuleRigidbody && ImGui::MenuItem("CapsuleRigidbody##new"))
			ENGINE.ENT_MNGR->AddComponent<CapsuleRigidbodyComponent>(_id);

		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
}

void DrawTransformProperty(u64 _id, const Icons& _icons)
{
	if (CreateNode<ECS::Components::TransformComponent>(_id, "Transform"))
	{
		auto& transformComp = ENGINE.ENT_MNGR->GetComponent<ECS::Components::TransformComponent>(_id);

		ImGui::Text("Position:");

		Maths::Vec3 pos = transformComp.position;
		if (DrawVec3Control("Position", pos))
			transformComp.position = pos;

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Rotation:");

		Maths::Vec3 rot = transformComp.eulerDegRotation;
		if (DrawVec3Control("Rotation", rot))
			transformComp.eulerDegRotation = rot;

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Scale:");

		Maths::Vec3 sca = transformComp.scale;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 23);

		static bool bScaleLock = false;
		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.5, 0.5, 0.5, 0.5 });
		if (ImGui::ImageButton((bScaleLock ? _icons.lock : _icons.unlock),
			{ 19,19 }, { 0,1 }, { 1,0 }, 0))
		{
			bScaleLock = !bScaleLock;
		}
		ImGui::PopStyleColor(2);

		ImGui::SameLine();

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);

		if (DrawVec3Control("Scale", sca, 1.f, bScaleLock))
			transformComp.scale = sca;

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DrawMeshRendererProperty(u64 _id, const Icons& _icons)
{
	if (CreateNode<ECS::Components::MeshRendererComponent>(_id, "Mesh"))
	{
		auto& meshComp = ENGINE.ENT_MNGR->GetComponent<ECS::Components::MeshRendererComponent>(_id);
		bool bEnabled = meshComp.bEnabled;
		if (ImGui::Checkbox("Enabled", &bEnabled))
			meshComp.bEnabled = bEnabled;

		ImGui::Text(meshComp.renderer.p_mesh->path.string().c_str());

		if (ImGui::Button("change"))
		{
			ImGui::OpenPopup("meshList");
		}
		if (ImGui::BeginPopup("meshList", NULL)) {
			for (const auto& r : ENGINE.RES_MNGR->meshList)
			{
				if (ImGui::MenuItem(r->path.string().c_str()))
				{
					meshComp.renderer.p_mesh = std::reinterpret_pointer_cast<Resources::Mesh>(r);
				}
			}
			ImGui::EndPopup();
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Text("Materials");
		for (int i = 0; i < meshComp.renderer.p_materials.size(); i++)
		{
			using namespace Resources::Materials;

			ImGui::PushID(i);
			const auto& r = meshComp.renderer.p_materials[i];

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.45f, 0.1f, 0.1f, 1.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100);
			if (ImGui::ImageButton(_icons.trash, { 19,19 }, { 0,1 }, { 1,0 }, 2))
			{
				meshComp.renderer.p_materials.erase(meshComp.renderer.p_materials.begin() + i);
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopID();
				break;
			}
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();

			std::string matTypeStr = "Material: " + MaterialTypeToString(r->type);
			ImGui::Text(matTypeStr.c_str());//  path.string().c_str());

			if (ImGui::Button("change##material"))
			{
				ImGui::OpenPopup("materialList");
			}
			if (ImGui::BeginPopup("materialList", NULL))
			{
				if (ImGui::MenuItem("Unlit"))
				{
					r->type = MaterialType::Unlit;
				}
				if (ImGui::MenuItem("Phong"))
				{
					r->type = MaterialType::Phong;
				}

				if (ImGui::MenuItem("PBR"))
				{
					r->type = MaterialType::PBR;
				}

				ImGui::EndPopup();
			}

			if (r->type == MaterialType::Unset)
			{
				ImGui::PopID();
				continue;
			}

			static char textureType = -1;

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text("Diffuse:");
			ImGui::Text(r->diffuse ? r->diffuse->GetResourcePath().string().c_str() : "Not Set");
			if (r->diffuse)ImGui::Image((ImTextureID)r->diffuse->ID, { 32,32 }, { 0,1 }, { 1,0 });
			ImGui::SameLine();
			if (ImGui::Button("Change##Diffuse"))
			{
				textureType = 0;
				ImGui::OpenPopup("textList");
			}

			if (r->type >= MaterialType::Phong)
			{
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text("Specular:");
				ImGui::Text(r->specular ? r->specular->GetResourcePath().string().c_str() : "Not Set");
				if (r->specular)ImGui::Image((ImTextureID)r->specular->ID, { 32,32 }, { 0,1 }, { 1,0 });
				ImGui::SameLine();
				if (ImGui::Button("Change##Specular"))
				{
					textureType = 1;
					ImGui::OpenPopup("textList");
				}
			}

			if (r->type >= MaterialType::PBR)
			{
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text("Ao:");
				ImGui::Text(r->ao ? r->ao->GetResourcePath().string().c_str() : "Not Set");
				if (r->ao)ImGui::Image((ImTextureID)r->ao->ID, { 32,32 }, { 0,1 }, { 1,0 });
				ImGui::SameLine();
				if (ImGui::Button("Change##Ao"))
				{
					textureType = 2;
					ImGui::OpenPopup("textList");
				}

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text("Normal:");
				ImGui::Text(r->normal ? r->normal->GetResourcePath().string().c_str() : "Not Set");
				if (r->normal)ImGui::Image((ImTextureID)r->normal->ID, { 32,32 }, { 0,1 }, { 1,0 });
				ImGui::SameLine();
				if (ImGui::Button("Change##Normal"))
				{
					textureType = 3;
					ImGui::OpenPopup("textList");
				}

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text("Roughness:");
				ImGui::Text(r->roughness ? r->roughness->GetResourcePath().string().c_str() : "Not Set");
				if (r->roughness)ImGui::Image((ImTextureID)r->roughness->ID, { 32,32 }, { 0,1 }, { 1,0 });
				ImGui::SameLine();
				if (ImGui::Button("Change##Roughness"))
				{
					textureType = 4;
					ImGui::OpenPopup("textList");
				}
			}

			if (r->type >= MaterialType::Phong)
			{
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text("Shininess");
				ImGui::SameLine();
				float shininess = r->shininess;
				if (ImGui::DragFloat("##Shininess", &shininess))
				{
					r->shininess = shininess;
				}

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text("GammaCorrection");
				ImGui::SameLine();
				float gammaCorrection = r->gammaCorrection;
				if (ImGui::DragFloat("##GammaCorrection", &gammaCorrection))
				{
					r->gammaCorrection = gammaCorrection;
				}
			}

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Separator();

			if (ImGui::BeginPopup("textList", NULL)) {
				for (const auto& t : ENGINE.RES_MNGR->textureList)
				{
					if (TextureMenuItem(t->path.string().c_str(), (ImTextureID)t->ID, { 32,32 }, { 0,1 }, { 1,0 }))
					{
						switch (textureType)
						{
						case 0:
							r->diffuse = std::reinterpret_pointer_cast<Resources::Texture>(t);
							break;
						case 1:
							r->specular = std::reinterpret_pointer_cast<Resources::Texture>(t);
							break;
						case 2:
							r->ao = std::reinterpret_pointer_cast<Resources::Texture>(t);
							break;
						case 3:
							r->normal = std::reinterpret_pointer_cast<Resources::Texture>(t);
							break;
						case 4:
							r->roughness = std::reinterpret_pointer_cast<Resources::Texture>(t);
							break;
						default:
							break;
						}
					}
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100);
		if (ImGui::ImageButton(_icons.plus, { 19,19 }, { 0,1 }, { 1,0 }, 2))
		{
			meshComp.renderer.p_materials.push_back(std::make_shared<Resources::Material>());//potential leak?
		}
		ImGui::PopStyleVar();

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DrawTextRendererProperty(u64 _id, const Icons& _icons)
{
	if (CreateNode<ECS::Components::TextRendererComponent>(_id, "Text"))
	{
		ECS::Components::TextRendererComponent& textComp = ENGINE.ENT_MNGR->GetComponent<ECS::Components::TextRendererComponent>(_id);
		bool bEnabled = textComp.bEnabled;
		if (ImGui::Checkbox("Enabled", &bEnabled))
			textComp.bEnabled = bEnabled;

		Resources::FontPtr p_font = textComp.p_font;

		int fontSize = p_font->GetFontSize();
		ImGui::Text("Size: ");
		ImGui::SameLine();
		if (ImGui::InputInt("##Size", &fontSize))
		{
			fontSize = std::clamp(fontSize, 0, 1000);
			p_font->ChangeSize(0, fontSize);
		}

		ImGui::Text("Font :");
		ImGui::SameLine();
		ImGui::Text(p_font ? p_font->path.string().c_str() : "No font set.");

		if (ImGui::Button("change"))
		{
			ImGui::OpenPopup("Font List");
		}

		if (ImGui::BeginPopup("Font List", NULL)) {
			for (const auto& r : ENGINE.RES_MNGR->fontList)
			{
				if (ImGui::MenuItem(r->path.string().c_str()))
				{
					textComp.p_font = std::reinterpret_pointer_cast<Resources::Font>(r);
				}
			}
			ImGui::EndPopup();
		}

		ImGui::Spacing();
		ImGui::Spacing();

		std::string text = textComp.text;

		ImGui::Text("Text input :");
		if (StringInputText("##Name", &text))
			textComp.text = text;

		ImGui::Spacing();
		ImGui::Spacing();

		f32_4 textColor = textComp.color.ToFloat();
		if (ImGui::ColorEdit4("Color##text", &textColor.x, ImGuiColorEditFlags_NoInputs))
		{
			textComp.color = TNCColor::CreateRGBAFloat(textColor.x, textColor.y, textColor.z, textColor.w);
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Checkbox("3D", &textComp.b3D);
		ImGui::Spacing();
		ImGui::TreePop();
		int priority = textComp.priority;
		if (ImGui::InputInt("Priority", &priority))
		{
			textComp.priority = std::max(0, priority);
		}
	}

	ImGui::Separator();
}

void DrawDirectionalLightProperty(u64 _id, const Icons& _icons)
{
	if (CreateNode<ECS::Components::DirectionalLightComponent>(_id, "Directional Light"))
	{
		auto& lightComp = ENGINE.ENT_MNGR->GetComponent<ECS::Components::DirectionalLightComponent>(_id);
		bool bEnabled = lightComp.bEnabled;
		if (ImGui::Checkbox("Enabled", &bEnabled))
			lightComp.bEnabled = bEnabled;

		LowRenderer::Lights::DirectionalLight light = lightComp.light;

		f32_4 directionalColor = light.color.ToFloat();
		if (ImGui::ColorEdit3("Color##directional", &directionalColor.x, ImGuiColorEditFlags_NoInputs))
		{
			light.color = TNCColor::CreateRGBAFloat(directionalColor.x, directionalColor.y, directionalColor.z, directionalColor.w);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		bool bShadowUi = light.bShadow;
		if (ImGui::Checkbox("IsShadowed##directional", &bShadowUi))
		{
			light.bShadow = bShadowUi;
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float biasUi = light.bias;
		ImGui::Text("Bias");
		ImGui::SameLine();
		if (ImGui::DragFloat("##Biasdirectional", &biasUi, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
		{
			light.bias = std::max(0.f, biasUi);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float strengthUi = light.strength;
		ImGui::Text("Strength");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##Strengthdirectional", &strengthUi, 0.1f, 1.f))
		{
			light.strength = std::clamp(strengthUi, 0.f, 1.f);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DrawPointLightProperty(u64 _id, const Icons& _icons)
{
	if (CreateNode<ECS::Components::PointLightComponent>(_id, "Point Light"))
	{
		auto& lightComp = ENGINE.ENT_MNGR->GetComponent<ECS::Components::PointLightComponent>(_id);
		bool bEnabled = lightComp.bEnabled;
		if (ImGui::Checkbox("Enabled", &bEnabled))
			lightComp.bEnabled = bEnabled;

		LowRenderer::Lights::PointLight light = lightComp.light;

		f32_4 directionalColor = light.color.ToFloat();
		if (ImGui::ColorEdit3("Color##point", &directionalColor.x, ImGuiColorEditFlags_NoInputs))
		{
			light.color = TNCColor::CreateRGBAFloat(directionalColor.x, directionalColor.y, directionalColor.z, directionalColor.w);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		bool bShadowUi = light.bShadow;
		if (ImGui::Checkbox("IsShadowed##point", &bShadowUi))
		{
			light.bShadow = bShadowUi;
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float biasUi = light.bias;
		ImGui::Text("Bias");
		ImGui::SameLine();
		if (ImGui::DragFloat("##Biaspoint", &biasUi, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
		{
			light.bias = std::max(0.f, biasUi);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float strengthUi = light.strength;
		ImGui::Text("Strength");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##StrengthPoint", &strengthUi, 0.1f, 1.f))
		{
			light.strength = std::clamp(strengthUi, 0.f, 1.f);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float constantUi = light.constant;
		ImGui::Text("Constant");
		ImGui::SameLine();
		if (ImGui::DragFloat("##Constantpoint", &constantUi, 0.1f, 0.f, std::numeric_limits<float>::infinity(), "%.5f"))
		{
			light.constant = std::max(0.f, constantUi);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float linearUi = light.linear;
		ImGui::Text("Linear");
		ImGui::SameLine();
		if (ImGui::DragFloat("##Linearpoint", &linearUi, 0.1f, 0.f, std::numeric_limits<float>::infinity(), "%.5f"))
		{
			light.linear = std::max(0.f, linearUi);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float quadraticUi = light.quadratic;
		ImGui::Text("Quadratic");
		ImGui::SameLine();
		if (ImGui::DragFloat("##Quadraticpoint", &quadraticUi, 0.1f, 0.f, std::numeric_limits<float>::infinity(), "%.5f"))
		{
			light.quadratic = std::max(0.f, quadraticUi);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DrawSpotLightProperty(u64 _id, const Icons& _icons)
{
	if (CreateNode<ECS::Components::SpotLightComponent>(_id, "Spot Light"))
	{
		auto& lightComp = ENGINE.ENT_MNGR->GetComponent<ECS::Components::SpotLightComponent>(_id);
		bool bEnabled = lightComp.bEnabled;
		if (ImGui::Checkbox("Enabled", &bEnabled))
			lightComp.bEnabled = bEnabled;

		LowRenderer::Lights::SpotLight light = lightComp.light;

		f32_4 directionalColor = light.color.ToFloat();
		if (ImGui::ColorEdit3("Color##spot", &directionalColor.x, ImGuiColorEditFlags_NoInputs))
		{
			light.color = TNCColor::CreateRGBAFloat(directionalColor.x, directionalColor.y, directionalColor.z, directionalColor.w);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		bool bShadowUi = light.bShadow;
		if (ImGui::Checkbox("IsShadowed##spot", &bShadowUi))
		{
			light.bShadow = bShadowUi;
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float biasUi = light.bias;
		ImGui::Text("Bias");
		ImGui::SameLine();
		if (ImGui::DragFloat("##Biasspot", &biasUi, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
		{
			light.bias = std::max(0.f, biasUi);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float strengthUi = light.strength;
		ImGui::Text("Strength");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##StrengthSpot", &strengthUi, 0.1f, 1.f))
		{
			light.strength = std::clamp(strengthUi, 0.f, 1.f);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("CutOff");

		float inCutOffUi = light.inCutOff;
		float outCutOffUi = light.outCutOff;

		ImGui::Text("InCutOff");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##InCutOffspot", &inCutOffUi, 0.f, outCutOffUi))
		{
			light.inCutOff = std::clamp(inCutOffUi, 0.f, light.outCutOff);
			lightComp.light = light;
		}
		ImGui::Text("OutCutOff");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##OutCutOffspot", &outCutOffUi, light.inCutOff, 180.f))
		{
			light.outCutOff = std::clamp(outCutOffUi, inCutOffUi, 180.f);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float constantUi = light.constant;
		ImGui::Text("Constant");
		ImGui::SameLine();
		if (ImGui::DragFloat("##Constantspot", &constantUi, 0.1f, 0.f, std::numeric_limits<float>::infinity(), "%.5f"))
		{
			light.constant = std::max(0.f, constantUi);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float linearUi = light.linear;
		ImGui::Text("Linear");
		ImGui::SameLine();
		if (ImGui::DragFloat("##Linearspot", &linearUi, 0.1f, 0.f, std::numeric_limits<float>::infinity(), "%.5f"))
		{
			light.linear = std::max(0.f, linearUi);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float quadraticUi = light.quadratic;
		ImGui::Text("Quadratic");
		ImGui::SameLine();
		if (ImGui::DragFloat("##Quadraticspot", &quadraticUi, 0.1f, 0.f, std::numeric_limits<float>::infinity(), "%.5f"))
		{
			light.quadratic = std::max(0.f, quadraticUi);
			lightComp.light = light;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DrawCameraProperty(u64 _id, const Icons& _icons)
{
	if (CreateNode<ECS::Components::CameraComponent>(_id, "Cameras"))
	{
		auto& camComp = ENGINE.ENT_MNGR->GetComponent<ECS::Components::CameraComponent>(_id);
		bool bEnabled = camComp.bEnabled;
		if (ImGui::Checkbox("Enabled", &bEnabled))
			camComp.bEnabled = bEnabled;

		LowRenderer::Cameras::Camera cam = camComp.camera;

		float aspect = cam.aspect;
		ImGui::Text("Aspect");
		ImGui::SameLine();
		if (ImGui::DragFloat("##Aspect", &aspect, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
		{
			cam.aspect = std::max(0.f, aspect);
			cam.SetProjection();
			cam.ComputeViewProjection();
			camComp.camera = cam;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float fovY = cam.fovY * Maths::Constants::RAD2DEG;
		ImGui::Text("FovY");
		ImGui::SameLine();
		if (ImGui::DragFloat("##FovY", &fovY, 0.1f, 45.f, 180.f))
		{
			cam.fovY = std::max(0.f, fovY) * Maths::Constants::DEG2RAD;
			cam.SetProjection();
			cam.ComputeViewProjection();
			camComp.camera = cam;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float zNear = cam.zNear;
		float zFar = cam.zFar;
		ImGui::Text("ZNear");
		ImGui::SameLine();
		if (ImGui::DragFloat("##ZNear", &zNear, 0.1f, 0.001f, (zFar - 1.f)))
		{
			cam.zNear = std::max(0.f, zNear);
			cam.SetProjection();
			cam.ComputeViewProjection();
			camComp.camera = cam;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		//zFar already defined
		ImGui::Text("ZFar");
		ImGui::SameLine();
		if (ImGui::DragFloat("##ZFar", &zFar, 0.1f, zNear + 1.f, std::numeric_limits<float>::infinity()))
		{
			cam.zFar = std::max(0.f, zFar);
			cam.SetProjection();
			cam.ComputeViewProjection();
			camComp.camera = cam;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float orthoScale = cam.orthoScale;
		ImGui::Text("OrthoScale");
		ImGui::SameLine();
		if (ImGui::DragFloat("##OrthoScale", &orthoScale, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
		{
			cam.orthoScale = std::max(0.f, orthoScale);
			cam.SetProjection();
			cam.ComputeViewProjection();
			camComp.camera = cam;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		bool bOrthographic = !cam.bPerspectiveMode;
		if (ImGui::Checkbox("Orthographic", &bOrthographic))
		{
			cam.bPerspectiveMode = !bOrthographic;
			cam.SetProjection();
			cam.ComputeViewProjection();
			camComp.camera = cam;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		bool bUsed = cam.bUsed;
		if (ImGui::Checkbox("Used", &bUsed))
		{
			cam.bUsed = bUsed;
			ENGINE.ENT_MNGR->GetComponent<ECS::Components::CameraComponent>(_id).camera = cam;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TreePop();
	}
	ImGui::Separator();
}
#include "../Assets/Script/PlayerController.hpp"
#include "../Assets/Script/LightController.hpp"

void DrawScriptComponentProperty(u64 _id, const Icons& _icons)
{
	if (CreateNode<ECS::Components::CppScriptComponent>(_id, "Script"))
	{
		auto& cppComp = ENGINE.ENT_MNGR->GetComponent<ECS::Components::CppScriptComponent>(_id);
		bool bEnabled = cppComp.bEnabled;
		ImGui::Checkbox("Enabled", &bEnabled);

		ImGui::Spacing();
		ImGui::Spacing();
		std::string scriptTypeStr = "Script: " + cppComp.scriptTypeStr;

		ImGui::Text(scriptTypeStr.c_str());
		if (ImGui::Button("Change"))
		{
			ImGui::OpenPopup("ScriptList");
		}

		if (ImGui::BeginPopup("ScriptList", NULL))
		{
			if (ImGui::MenuItem("PlayerController"))
			{
				cppComp.UnBind();
				cppComp.Bind<PlayerController>();
			}
			if (ImGui::MenuItem("LightController"))
			{
				cppComp.UnBind();
				cppComp.Bind<LightController>();
			}
			ImGui::EndPopup();
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DrawAudioSourceProperty(u64 _id, const Icons& _icons)
{
	if (CreateNode<ECS::Components::AudioSourceComponent>(_id, "AudioSource"))
	{
		auto& aSourceComp = ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioSourceComponent>(_id);
		bool bEnabled = aSourceComp.bEnabled;
		if (ImGui::Checkbox("Enabled", &bEnabled))
		{
			if (!aSourceComp.bEnabled)
				aSourceComp.Stop();
			else if (aSourceComp.bPlayOnStart)
				aSourceComp.Play();
		}
		float volume = aSourceComp.volume;
		ImGui::Text("Volume");
		ImGui::SameLine();
		if (ImGui::DragFloat("##Volume", &volume, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
		{
			aSourceComp.volume = std::max(0.f, volume);
		}

		ImGui::Spacing();
		ImGui::Spacing();

		float pitch = aSourceComp.pitch;
		ImGui::Text("Pitch");
		ImGui::SameLine();
		if (ImGui::DragFloat("##Pitch", &pitch, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
		{
			aSourceComp.pitch = std::max(0.f, pitch);
		}

		ImGui::Spacing();
		ImGui::Spacing();

		bool bSpatialized = aSourceComp.bSpatialized;
		if (ImGui::Checkbox("Spatialized", &bSpatialized))
		{
			aSourceComp.bSpatialized = bSpatialized;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		bool bLoop = aSourceComp.bLoop;
		if (ImGui::Checkbox("Loop", &bLoop))
		{
			aSourceComp.bLoop = bLoop;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		bool bPlayOnStart = aSourceComp.bPlayOnStart;
		if (ImGui::Checkbox("PlayOnStart", &bPlayOnStart))
		{
			aSourceComp.bPlayOnStart = bPlayOnStart;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		Resources::SoundPtr p_sound = aSourceComp.p_sound;

		ImGui::Text("Audio :");
		ImGui::SameLine();
		ImGui::Text(p_sound ? p_sound->path.string().c_str() : "No Audio set.");

		if (ImGui::Button("Change"))
		{
			ImGui::OpenPopup("AudioList");
		}

		if (p_sound)
		{
			if (ImGui::Button("Play"))
				p_sound->Play();

			ImGui::SameLine();

			if (ImGui::Button("Pause"))
				p_sound->Pause();

			ImGui::SameLine();

			if (ImGui::Button("Stop"))
				p_sound->Stop();
		}

		if (ImGui::BeginPopup("AudioList", NULL)) {
			for (const auto& r : ENGINE.RES_MNGR->audioList)
			{
				if (ImGui::MenuItem(r->path.string().c_str()))
				{
					p_sound = std::reinterpret_pointer_cast<Resources::Sound>(r);
					aSourceComp.p_sound = p_sound;
				}
			}
			ImGui::EndPopup();
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DrawAudioListenerProperty(u64 _id, const Icons& _icons)
{
	if (CreateNode<ECS::Components::AudioListenerComponent>(_id, "AudioListener"))
	{
		auto& aListenerComp = ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioListenerComponent>(_id);

		bool bEnabled = aListenerComp.bEnabled;
		if (ImGui::Checkbox("Enabled", &bEnabled))
			aListenerComp.bEnabled = bEnabled;

		ImGui::Text("Yes");
		//TODO Edit cone
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DrawBoxRigidbodyProperty(u64 _id, const Icons& _icons)
{
	if (CreateNode<ECS::Components::BoxRigidbodyComponent>(_id, "BoxRigidbody"))
	{
		ECS::Components::BoxRigidbodyComponent& rigidbodyComp
			= ENGINE.ENT_MNGR->GetComponent<ECS::Components::BoxRigidbodyComponent>(_id);

		if (!rigidbodyComp.RigidbodyExist())
		{
			ImGui::TreePop();
			ImGui::Separator();
			return;
		}

		/* Enable */ {
			ImGui::Text("Enabled");
			ImGui::SameLine();

			bool enabled = rigidbodyComp.bEnabled;
			if (ImGui::Checkbox("##Enabled", &enabled))
				rigidbodyComp.bEnabled = enabled;

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Position */ {
			Maths::Vec3 center = rigidbodyComp.position;
			ImGui::Text("Position :\n x = % .3f, y = % .3f, z = % .3f", center.x, center.y, center.z);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Position (center offset) */ {
			ImGui::Text("Position Offset");
			ImGui::SameLine();

			Maths::Vec3 position = rigidbodyComp.centerOffset;
			if (DrawVec3Control("##Position Offset", position))
				rigidbodyComp.centerOffset = position;

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Rotation */ {
			Maths::Quat rotation = rigidbodyComp.rotation;
			Maths::Vec3 rot = rotation.ToEulerAngles() * Maths::Constants::RAD2DEG;
			ImGui::Text("Rotation :\n x = %.3f, y = %.3f, z = %.3f", rot.x, rot.y, rot.z);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Rotation offset*/ {
			ImGui::Text("Rotation Offset");
			ImGui::SameLine();

			Maths::Vec3 rotOff = rigidbodyComp.rotationOffset.ToEulerAngles() * Maths::Constants::RAD2DEG;
			if (DrawVec3Control("##Rotation", rotOff))
				rigidbodyComp.rotationOffset = Maths::Quaternions::FromEulerAngles(rotOff * Maths::Constants::DEG2RAD);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Box Extents*/ {
			ImGui::Text("Extents");
			ImGui::SameLine();

			Maths::Vec3 extents = rigidbodyComp.extents / 2.f;
			if (DrawVec3Control("##Extents", extents, 1.f, true))
			{
				extents *= 2.f;
				rigidbodyComp.extents = Maths::Vec3(std::max(extents.x, .25f), std::max(extents.y, .25f), std::max(extents.z, .25f));
			}

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Motion Type*/ {
			ImGui::Text("Motion Type:");
			ImGui::SameLine();

			Physics::Motions::MotionType motionType = rigidbodyComp.motionType;
			ImGui::Text(Physics::Motions::MotionTypeToString(motionType).c_str());
			if (ImGui::Button("Change"))
				ImGui::OpenPopup("MotionTypes");

			if (ImGui::BeginPopup("MotionTypes"))
			{
				if (ImGui::MenuItem("Static"))
					rigidbodyComp.motionType = Physics::Motions::MotionType::Static;
				if (ImGui::MenuItem("Kinematic"))
					rigidbodyComp.motionType = Physics::Motions::MotionType::Kinematic;
				if (ImGui::MenuItem("Dynamic"))
					rigidbodyComp.motionType = Physics::Motions::MotionType::Dynamic;
				ImGui::EndPopup();
			}

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Angular Velocity*/ {
			Maths::Vec3 angVelocity = rigidbodyComp.angularVelocity;
			ImGui::Text("Angular velocity :\n x = %.3f, y = %.3f, z = %.3f", angVelocity.x, angVelocity.y, angVelocity.z);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Linear Velocity*/ {
			Maths::Vec3 linVelocity = rigidbodyComp.linearVelocity;
			ImGui::Text("Linear velocity :\n x = %.3f, y = %.3f, z = %.3f", linVelocity.x, linVelocity.y, linVelocity.z);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Reset Velocity*/ {
			if (ImGui::Button("Reset velocity"))
			{
				rigidbodyComp.angularVelocity = Maths::Vec3(0);
				rigidbodyComp.linearVelocity = Maths::Vec3(0);
			}

			ImGui::Spacing();
			ImGui::Spacing();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DrawSphereRigidbodyProperty(u64 _id, const Icons& _icons)
{
	if (CreateNode<ECS::Components::SphereRigidbodyComponent>(_id, "SphereRigidbody"))
	{
		ECS::Components::SphereRigidbodyComponent& rigidbodyComp
			= ENGINE.ENT_MNGR->GetComponent<ECS::Components::SphereRigidbodyComponent>(_id);

		if (!rigidbodyComp.RigidbodyExist())
		{
			ImGui::TreePop();
			ImGui::Separator();
			return;
		}

		/* Enable */ {
			bool enabled = rigidbodyComp.bEnabled;
			ImGui::Text("Enabled");
			ImGui::SameLine();

			if (ImGui::Checkbox("##Enabled", &enabled))
				rigidbodyComp.bEnabled = enabled;

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Position */ {
			Maths::Vec3 center = rigidbodyComp.position;
			ImGui::Text("Position :\n x = % .3f, y = % .3f, z = % .3f", center.x, center.y, center.z);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Position (center offset)*/ {
			ImGui::Text("Position Offset");
			ImGui::SameLine();

			Maths::Vec3 position = rigidbodyComp.centerOffset;
			if (DrawVec3Control("##Position Offset", position))
				rigidbodyComp.centerOffset = position;

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Rotation */ {
			Maths::Quat rotation = rigidbodyComp.rotation;
			Maths::Vec3 rot = rotation.ToEulerAngles() * Maths::Constants::RAD2DEG;
			ImGui::Text("Rotation :\n x = %.3f, y = %.3f, z = %.3f", rot.x, rot.y, rot.z);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Rotation offset*/ {
			ImGui::Text("Rotation Offset");
			ImGui::SameLine();

			Maths::Vec3 rotOff = rigidbodyComp.rotationOffset.ToEulerAngles() * Maths::Constants::RAD2DEG;
			if (DrawVec3Control("##Rotation", rotOff))
				rigidbodyComp.rotationOffset = Maths::Quaternions::FromEulerAngles(rotOff * Maths::Constants::DEG2RAD);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Bounds Radius*/ {
			ImGui::Text("Radius");
			ImGui::SameLine();

			float radius = rigidbodyComp.radius;
			if (ImGui::DragFloat("##Radius", &radius, .01f, .1f, std::numeric_limits<float>::max()))
				rigidbodyComp.radius = std::max(radius, .15f);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Motion Type*/ {
			ImGui::Text("Motion Type:");
			ImGui::SameLine();

			Physics::Motions::MotionType motionType = rigidbodyComp.motionType;
			ImGui::Text(Physics::Motions::MotionTypeToString(motionType).c_str());
			if (ImGui::Button("Change"))
				ImGui::OpenPopup("MotionTypes");

			if (ImGui::BeginPopup("MotionTypes"))
			{
				if (ImGui::MenuItem("Static"))
					rigidbodyComp.motionType = Physics::Motions::MotionType::Static;
				if (ImGui::MenuItem("Kinematic"))
					rigidbodyComp.motionType = Physics::Motions::MotionType::Kinematic;
				if (ImGui::MenuItem("Dynamic"))
					rigidbodyComp.motionType = Physics::Motions::MotionType::Dynamic;
				ImGui::EndPopup();
			}

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Angular Velocity*/ {
			Maths::Vec3 angVelocity = rigidbodyComp.angularVelocity;
			ImGui::Text("Angular velocity :\n x = %.3f, y = %.3f, z = %.3f", angVelocity.x, angVelocity.y, angVelocity.z);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Linear Velocity*/ {
			Maths::Vec3 linVelocity = rigidbodyComp.linearVelocity;
			ImGui::Text("Linear velocity :\n x = %.3f, y = %.3f, z = %.3f", linVelocity.x, linVelocity.y, linVelocity.z);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Reset Velocity*/ {
			if (ImGui::Button("Reset velocity"))
			{
				rigidbodyComp.angularVelocity = Maths::Vec3(0);
				rigidbodyComp.linearVelocity = Maths::Vec3(0);
			}

			ImGui::Spacing();
			ImGui::Spacing();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
}

void DrawCapsuleRigidbodyProperty(u64 _id, const Icons& _icons)
{
	if (CreateNode<ECS::Components::CapsuleRigidbodyComponent>(_id, "CapsuleRigidbody"))
	{
		ECS::Components::CapsuleRigidbodyComponent& rigidbodyComp
			= ENGINE.ENT_MNGR->GetComponent<ECS::Components::CapsuleRigidbodyComponent>(_id);

		if (!rigidbodyComp.RigidbodyExist())
		{
			ImGui::TreePop();
			ImGui::Separator();
			return;
		}

		/* Enable */ {
			bool enabled = rigidbodyComp.bEnabled;
			ImGui::Text("Enabled");

			ImGui::SameLine();
			if (ImGui::Checkbox("##Enabled", &enabled))
				rigidbodyComp.bEnabled = enabled;

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Position */ {
			Maths::Vec3 center = rigidbodyComp.position;
			ImGui::Text("Position :\n x = % .3f, y = % .3f, z = % .3f", center.x, center.y, center.z);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Position (center offset)*/ {
			ImGui::Text("Position Offset");
			ImGui::SameLine();

			Maths::Vec3 position = rigidbodyComp.centerOffset;
			if (DrawVec3Control("##Position Offset", position))
				rigidbodyComp.centerOffset = position;

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Rotation */ {
			Maths::Quat rotation = rigidbodyComp.rotation;
			Maths::Vec3 rot = rotation.ToEulerAngles() * Maths::Constants::RAD2DEG;
			ImGui::Text("Rotation :\n x = %.3f, y = %.3f, z = %.3f", rot.x, rot.y, rot.z);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Rotation offset*/ {
			ImGui::Text("Rotation Offset");
			ImGui::SameLine();

			Maths::Vec3 rotOff = rigidbodyComp.rotationOffset.ToEulerAngles() * Maths::Constants::RAD2DEG;
			if (DrawVec3Control("##Rotation", rotOff))
				rigidbodyComp.rotationOffset = Maths::Quaternions::FromEulerAngles(rotOff * Maths::Constants::DEG2RAD);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Bounds Radius*/ {
			ImGui::Text("Radius");
			ImGui::SameLine();

			float radius = rigidbodyComp.radius;
			if (ImGui::DragFloat("##Radius", &radius, .01f, .15f, std::numeric_limits<float>::max()))
				rigidbodyComp.radius = std::max(radius, .15f);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Bounds Height*/ {
			ImGui::Text("Height");
			ImGui::SameLine();

			float height = rigidbodyComp.height;
			if (ImGui::DragFloat("##Height", &height, .01f, .2f, std::numeric_limits<float>::max()))
				rigidbodyComp.height = std::max(height, .2f);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Motion Type*/ {
			ImGui::Text("Motion Type:");
			ImGui::SameLine();

			Physics::Motions::MotionType motionType = rigidbodyComp.motionType;
			ImGui::Text(Physics::Motions::MotionTypeToString(motionType).c_str());
			if (ImGui::Button("Change"))
				ImGui::OpenPopup("MotionTypes");

			if (ImGui::BeginPopup("MotionTypes"))
			{
				if (ImGui::MenuItem("Static"))
					rigidbodyComp.motionType = Physics::Motions::MotionType::Static;
				if (ImGui::MenuItem("Kinematic"))
					rigidbodyComp.motionType = Physics::Motions::MotionType::Kinematic;
				if (ImGui::MenuItem("Dynamic"))
					rigidbodyComp.motionType = Physics::Motions::MotionType::Dynamic;
				ImGui::EndPopup();
			}

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Angular Velocity*/ {
			Maths::Vec3 angVelocity = rigidbodyComp.angularVelocity;
			ImGui::Text("Angular velocity :\n x = %.3f, y = %.3f, z = %.3f", angVelocity.x, angVelocity.y, angVelocity.z);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Linear Velocity*/ {
			Maths::Vec3 linVelocity = rigidbodyComp.linearVelocity;
			ImGui::Text("Linear velocity :\n x = %.3f, y = %.3f, z = %.3f", linVelocity.x, linVelocity.y, linVelocity.z);

			ImGui::Spacing();
			ImGui::Spacing();
		}
		/* Reset Velocity*/ {
			if (ImGui::Button("Reset velocity"))
			{
				rigidbodyComp.angularVelocity = Maths::Vec3(0);
				rigidbodyComp.linearVelocity = Maths::Vec3(0);
			}

			ImGui::Spacing();
			ImGui::Spacing();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
}