#include "Core/EditorWindow.hpp"
#include "Core/Utils.hpp"

#include <ECS/Components.hpp>

template<typename Component>
bool CreateNode(u64 _id, const char* name)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    float y1 = (ImGui::GetCursorScreenPos().y - 2);// +(23 * (int)row_n);
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
        ImGui::TreePop();
        return false;
    }
    ImGui::PopStyleColor(2);
    ImGui::PopID();

    return open;
}

void Core::Applications::EditorWindow::DrawProperty(u64 _id)
{
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

	bool hasTransform = ENGINE.ENT_MNGR->HasComponent<ECS::Components::TransformComponent>(_id);
	bool hasMeshRenderer = ENGINE.ENT_MNGR->HasComponent<ECS::Components::MeshRendererComponent>(_id);
	bool hasDirectionalLight = ENGINE.ENT_MNGR->HasComponent<ECS::Components::DirectionalLightComponent>(_id);
	bool hasPointLight = ENGINE.ENT_MNGR->HasComponent<ECS::Components::PointLightComponent>(_id);
	bool hasSpotLight = ENGINE.ENT_MNGR->HasComponent<ECS::Components::SpotLightComponent>(_id);
	bool hasCamera = ENGINE.ENT_MNGR->HasComponent<ECS::Components::CameraComponent>(_id);
	bool hasAudioSource = ENGINE.ENT_MNGR->HasComponent<ECS::Components::AudioSourceComponent>(_id);
	bool hasAudioListener = ENGINE.ENT_MNGR->HasComponent<ECS::Components::AudioListenerComponent>(_id);



	if (hasTransform)
	{
		bool open = CreateNode<ECS::Components::TransformComponent>(_id, "Transform");

        if (open)
        {
            ImGui::Text("Position:");

			Maths::Vec3 pos = ENGINE.ENT_MNGR->GetComponent<ECS::Components::TransformComponent>(_id).position;
			if (DrawVec3Control("Position", pos))
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::TransformComponent>(_id).position = pos;

			ImGui::Spacing();
			ImGui::Spacing();


			ImGui::Text("Rotation:");

			static Maths::Vec3 rot = ENGINE.ENT_MNGR->GetComponent<ECS::Components::TransformComponent>(_id).eulerDegRotation;
			if (DrawVec3Control("Rotation", rot))
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::TransformComponent>(_id).eulerDegRotation = rot;

			ImGui::Spacing();
			ImGui::Spacing();


			ImGui::Text("Scale:");

			Maths::Vec3 sca = ENGINE.ENT_MNGR->GetComponent<ECS::Components::TransformComponent>(_id).scale;

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 23);

			static bool bScaleLock = false;
			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.5, 0.5, 0.5, 0.5 });
			if (ImGui::ImageButton((ImTextureID)ENGINE.RES_MNGR->Get<Resources::Texture>(bScaleLock ? "StaticAssets\\Images\\lock.png" : "StaticAssets\\Images\\unlock.png")->ID,
				{ 19,19 }, { 0,1 }, { 1,0 }, 0))
			{
				bScaleLock = !bScaleLock;
			}
			ImGui::PopStyleColor(2);

			ImGui::SameLine();

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);


			if (DrawVec3Control("Scale", sca, 1.f, bScaleLock))
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::TransformComponent>(_id).scale = sca;

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::TreePop();
		}

        ImGui::Separator();
    }

	if (hasMeshRenderer)
	{
		bool open = CreateNode<ECS::Components::MeshRendererComponent>(_id, "Mesh");

        if (open)
        {

            /*ImVec2 Start = ImGui::GetCursorScreenPos();
            drawList->AddText({ 0,0 }, 0xFFFFFF, "Position");*/

			ImGui::Text(ENGINE.ENT_MNGR->GetComponent<ECS::Components::MeshRendererComponent>(_id).renderer.p_mesh->path.string().c_str());
			
			if (ImGui::Button("change"))
			{
				ImGui::OpenPopup("meshList");
			}
			if (ImGui::BeginPopup("meshList", NULL)) {

				for (const auto& r : ENGINE.RES_MNGR->meshList)
				{
					if (ImGui::MenuItem(r->path.string().c_str()))
					{
						ENGINE.ENT_MNGR->GetComponent<ECS::Components::MeshRendererComponent>(_id).renderer.p_mesh = std::reinterpret_pointer_cast<Resources::Mesh>(r);
					}

                }
                ImGui::EndPopup();
            }
            
            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::Separator();

            for (int i = 0; i < ENGINE.ENT_MNGR->GetComponent<ECS::Components::MeshRendererComponent>(_id).renderer.p_materials.size(); i++)
            {
                ImGui::PushID(i);
                const auto& r = ENGINE.ENT_MNGR->GetComponent<ECS::Components::MeshRendererComponent>(_id).renderer.p_materials[i];

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.45f, 0.1f, 0.1f, 1.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100);
                if (ImGui::ImageButton((ImTextureID)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\trash.png")->ID,
                    { 19,19 }, { 0,1 }, { 1,0 }, 2))
                {
                    ENGINE.ENT_MNGR->GetComponent<ECS::Components::MeshRendererComponent>(_id).renderer.p_materials.erase(ENGINE.ENT_MNGR->GetComponent<ECS::Components::MeshRendererComponent>(_id).renderer.p_materials.begin() + i);
                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();
                    ImGui::PopID();
                    break;
                }
                ImGui::PopStyleVar();
                ImGui::PopStyleColor();

                std::string matTypeStr = "Material: " + Resources::MaterialTypeToString(r->type);
                ImGui::Text(matTypeStr.c_str());//  path.string().c_str());

                if (ImGui::Button("change##material"))
                {
                    ImGui::OpenPopup("materialList");
                }
                if (ImGui::BeginPopup("materialList", NULL))
                {
                    if (ImGui::MenuItem("Unlit"))
                    {
                        r->type = Resources::MaterialType::Unlit;
                    }
                    if (ImGui::MenuItem("Phong"))
                    {
                        r->type = Resources::MaterialType::Phong;
                    }

                    if (ImGui::MenuItem("PBR"))
                    {
                        r->type = Resources::MaterialType::PBR;
                    }

                    ImGui::EndPopup();
                }

                if (r->type == Resources::MaterialType::Unset)
                {
                    ImGui::PopID();
                    continue;
                }

              
                static char textureType = -1;


                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::Text("Diffuse:");
                ImGui::Text(r->diffuse ? r->diffuse->GetResourcePath().string().c_str() : "Not Set");
                if(r->diffuse)ImGui::Image((ImTextureID)r->diffuse->ID, { 32,32 }, { 0,1 }, { 1,0 });
                ImGui::SameLine();
                if (ImGui::Button("Change##Diffuse"))
                {
                    textureType = 0;
                    ImGui::OpenPopup("textList");
                }

                if (r->type >= Resources::MaterialType::Phong)
                {
                    ImGui::Spacing();
                    ImGui::Spacing();

                    ImGui::Text("Specular:");
                    ImGui::Text(r->specular ? r->specular->GetResourcePath().string().c_str() : "Not Set");
                    if (r->specular)ImGui::Image((ImTextureID)r->specular->ID, { 32,32 }, { 0,1 }, { 1,0 });
                    ImGui::SameLine();
                    if(ImGui::Button("Change##Specular"))
                    {
                        textureType = 1;
                        ImGui::OpenPopup("textList");
                    }
                }

                if (r->type >= Resources::MaterialType::PBR)
                {
                    ImGui::Spacing();
                    ImGui::Spacing();

                    ImGui::Text("Ao:");
                    ImGui::Text(r->ao ? r->ao->GetResourcePath().string().c_str() : "Not Set");
                    if (r->ao)ImGui::Image((ImTextureID)r->ao->ID, { 32,32 }, { 0,1 }, { 1,0 });
                    ImGui::SameLine();
                    if(ImGui::Button("Change##Ao"))
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
                    if(ImGui::Button("Change##Normal"))
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
                    if(ImGui::Button("Change##Roughness"))
                    {
                        textureType = 4;
                        ImGui::OpenPopup("textList");
                    }
                }

                if (r->type >= Resources::MaterialType::Phong)
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
                //ImGui::Text(r->path.string().c_str());
                ImGui::PopID();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100);
            if (ImGui::ImageButton((ImTextureID)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\plus.png")->ID,
                { 19,19 }, { 0,1 }, { 1,0 }, 2))
            {
                ENGINE.ENT_MNGR->GetComponent<ECS::Components::MeshRendererComponent>(_id).renderer.p_materials.push_back(std::make_shared<Resources::Material>());//potential leak?
            }
            ImGui::PopStyleVar();

            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::TreePop();
        }
        ImGui::Separator();
    }

	if (hasDirectionalLight)
	{
		bool open = CreateNode<ECS::Components::DirectionalLightComponent>(_id, "Directional Light");

		if (open)
		{
			LowRenderer::Lights::DirectionalLight light = ENGINE.ENT_MNGR->GetComponent<ECS::Components::DirectionalLightComponent>(_id).light;

			f32_4 directionalColor = light.color.ToFloat();
			if (ImGui::ColorEdit3("Color##directional", &directionalColor.x, ImGuiColorEditFlags_NoInputs))
			{
				light.color = TNCColor::CreateRGBAFloat(directionalColor.x, directionalColor.y, directionalColor.z, directionalColor.w);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::DirectionalLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			bool bShadowUi = light.bShadow;
			if (ImGui::Checkbox("IsShadowed##directional", &bShadowUi))
			{
				light.bShadow = bShadowUi;
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::DirectionalLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float biasUi = light.bias;
			ImGui::Text("Bias");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Biasdirectional", &biasUi, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
			{
				light.bias = std::max(0.f, biasUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::DirectionalLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float strengthUi = light.strength;
			ImGui::Text("Strength");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Strengthdirectional", &strengthUi, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
			{
				light.strength = std::max(0.f, strengthUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::DirectionalLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			ImGui::TreePop();
		}
		ImGui::Separator();
	}
	if (hasPointLight)
	{
		bool open = CreateNode<ECS::Components::PointLightComponent>(_id, "Point Light");

		if (open)
		{
			LowRenderer::Lights::PointLight light = ENGINE.ENT_MNGR->GetComponent<ECS::Components::PointLightComponent>(_id).light;

			f32_4 directionalColor = light.color.ToFloat();
			if (ImGui::ColorEdit3("Color##point", &directionalColor.x, ImGuiColorEditFlags_NoInputs))
			{
				light.color = TNCColor::CreateRGBAFloat(directionalColor.x, directionalColor.y, directionalColor.z, directionalColor.w);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::PointLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			bool bShadowUi = light.bShadow;
			if (ImGui::Checkbox("IsShadowed##point", &bShadowUi))
			{
				light.bShadow = bShadowUi;
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::PointLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float biasUi = light.bias;
			ImGui::Text("Bias");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Biaspoint", &biasUi, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
			{
				light.bias = std::max(0.f, biasUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::PointLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float strengthUi = light.strength;
			ImGui::Text("Strength");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Strengthpoint", &strengthUi, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
			{
				light.strength = std::max(0.f, strengthUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::PointLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float constantUi = light.constant;
			ImGui::Text("Constant");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Constantpoint", &constantUi, 0.1f, 0.f, std::numeric_limits<float>::infinity(), "%.5f"))
			{
				light.constant = std::max(0.f, constantUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::PointLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float linearUi = light.linear;
			ImGui::Text("Linear");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Linearpoint", &linearUi, 0.1f, 0.f, std::numeric_limits<float>::infinity(), "%.5f"))
			{
				light.linear = std::max(0.f, linearUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::PointLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float quadraticUi = light.quadratic;
			ImGui::Text("Quadratic");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Quadraticpoint", &quadraticUi, 0.1f, 0.f, std::numeric_limits<float>::infinity(),"%.5f"))
			{
				light.quadratic = std::max(0.f, quadraticUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::PointLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			ImGui::TreePop();
		}
		ImGui::Separator();
	}
	if (hasSpotLight)
	{
		bool open = CreateNode<ECS::Components::SpotLightComponent>(_id, "Spot Light");

		if (open)
		{
			LowRenderer::Lights::SpotLight light = ENGINE.ENT_MNGR->GetComponent<ECS::Components::SpotLightComponent>(_id).light;

			f32_4 directionalColor = light.color.ToFloat();
			if (ImGui::ColorEdit3("Color##spot", &directionalColor.x, ImGuiColorEditFlags_NoInputs))
			{
				light.color = TNCColor::CreateRGBAFloat(directionalColor.x, directionalColor.y, directionalColor.z, directionalColor.w);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::SpotLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			bool bShadowUi = light.bShadow;
			if (ImGui::Checkbox("IsShadowed##spot", &bShadowUi))
			{
				light.bShadow = bShadowUi;
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::SpotLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float biasUi = light.bias;
			ImGui::Text("Bias");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Biasspot", &biasUi, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
			{
				light.bias = std::max(0.f, biasUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::SpotLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float strengthUi = light.strength;
			ImGui::Text("Strength");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Strengthspot", &strengthUi, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
			{
				light.strength = std::max(0.f, strengthUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::SpotLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float inCutOffUi = light.inCutOff;
			ImGui::Text("InCutOff");
			ImGui::SameLine();
			if (ImGui::DragFloat("##InCutOffspot", &inCutOffUi, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
			{
				light.inCutOff = std::max(0.f, inCutOffUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::SpotLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float outCutOffUi = light.outCutOff;
			ImGui::Text("OutCutOff");
			ImGui::SameLine();
			if (ImGui::DragFloat("##OutCutOffspot", &outCutOffUi, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
			{
				light.outCutOff = std::max(0.f, outCutOffUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::SpotLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float constantUi = light.constant;
			ImGui::Text("Constant");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Constantspot", &constantUi, 0.1f, 0.f, std::numeric_limits<float>::infinity(), "%.5f"))
			{
				light.constant = std::max(0.f, constantUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::SpotLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float linearUi = light.linear;
			ImGui::Text("Linear");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Linearspot", &linearUi, 0.1f, 0.f, std::numeric_limits<float>::infinity(), "%.5f"))
			{
				light.linear = std::max(0.f, linearUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::SpotLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float quadraticUi = light.quadratic;
			ImGui::Text("Quadratic");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Quadraticspot", &quadraticUi, 0.1f, 0.f, std::numeric_limits<float>::infinity(), "%.5f"))
			{
				light.quadratic = std::max(0.f, quadraticUi);
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::SpotLightComponent>(_id).light = light;
			}

            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::TreePop();
        }
        ImGui::Separator();
    }

	if (hasCamera)
	{
		bool open = CreateNode<ECS::Components::CameraComponent>(_id, "Cameras");

		if (open)
		{
			LowRenderer::Cameras::Camera cam = ENGINE.ENT_MNGR->GetComponent<ECS::Components::CameraComponent>(_id).camera;

            /*ImVec2 Start = ImGui::GetCursorScreenPos();
            drawList->AddText({ 0,0 }, 0xFFFFFF, "Position");*/

			float aspect = cam.aspect;
			ImGui::Text("Aspect");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Aspect", &aspect, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
			{
				cam.aspect = std::max(0.f, aspect);
				cam.SetProjection();
				cam.ComputeViewProjection();
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::CameraComponent>(_id).camera = cam;
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
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::CameraComponent>(_id).camera = cam;
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
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::CameraComponent>(_id).camera = cam;
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
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::CameraComponent>(_id).camera = cam;
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
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::CameraComponent>(_id).camera = cam;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			bool bOrthographic = cam.bPerspectiveMode;
			if (ImGui::Checkbox("Orthographic", &bOrthographic))
			{
				cam.bPerspectiveMode = bOrthographic;
				cam.SetProjection();
				cam.ComputeViewProjection();
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::CameraComponent>(_id).camera = cam;
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

	if (hasAudioSource)
	{
		bool open = CreateNode<ECS::Components::AudioSourceComponent>(_id, "AudioSource");

		if (open)
		{
			float volume = ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioSourceComponent>(_id).volume;
			ImGui::Text("Volume");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Volume", &volume, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
			{
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioSourceComponent>(_id).volume = std::max(0.f, volume);
			}

            ImGui::Spacing();
            ImGui::Spacing();

			float pitch = ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioSourceComponent>(_id).pitch;
			ImGui::Text("Pitch");
			ImGui::SameLine();
			if (ImGui::DragFloat("##Pitch", &pitch, 0.1f, 0.f, std::numeric_limits<float>::infinity()))
			{
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioSourceComponent>(_id).pitch = std::max(0.f, pitch);
			}

            ImGui::Spacing();
            ImGui::Spacing();

			bool bSpatialized = ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioSourceComponent>(_id).bSpatialized;
			if (ImGui::Checkbox("Spatialized", &bSpatialized))
			{
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioSourceComponent>(_id).bSpatialized = bSpatialized;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			bool bLoop = ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioSourceComponent>(_id).bLoop;
			if (ImGui::Checkbox("Loop", &bLoop))
			{
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioSourceComponent>(_id).bLoop = bLoop;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			bool bPlayOnStart = ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioSourceComponent>(_id).bPlayOnStart;
			if (ImGui::Checkbox("PlayOnStart", &bPlayOnStart))
			{
				ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioSourceComponent>(_id).bPlayOnStart = bPlayOnStart;
			}

            ImGui::Spacing();
            ImGui::Spacing();

			Resources::SoundPtr p_sound = ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioSourceComponent>(_id).p_sound;

            ImGui::Text("Audio :");
            ImGui::SameLine();
            ImGui::Text(p_sound ? p_sound->path.string().c_str() : "No Audio set.");

            if (ImGui::Button("change"))
            {
                ImGui::OpenPopup("audioList");
            }

            if (p_sound)
            {
                if (ImGui::Button("play"))
                    p_sound->Play();

                ImGui::SameLine();

                if (ImGui::Button("pause"))
                    p_sound->Pause();

                ImGui::SameLine();

                if (ImGui::Button("stop"))
                    p_sound->Stop();
            }

            if (ImGui::BeginPopup("audioList", NULL)) {

				for (const auto& r : ENGINE.RES_MNGR->audioList)
				{
					if (ImGui::MenuItem(r->path.string().c_str()))
					{
						p_sound = std::reinterpret_pointer_cast<Resources::Sound>(r);
						ENGINE.ENT_MNGR->GetComponent<ECS::Components::AudioSourceComponent>(_id).p_sound = p_sound;
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
	if (hasAudioListener)
	{
		bool open = CreateNode<ECS::Components::AudioListenerComponent>(_id, "AudioListener");

        if (open)
        {
            ImGui::Text("Yes");

            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::TreePop();
        }
        ImGui::Separator();
    }




    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100);
    //ImVec2 addButtonSize = ImGui::CalcTextSize("Add...");
    //ImGui::Button("Add...", { addButtonSize.x + 30 , addButtonSize.y + 10 });
    if (ImGui::ImageButton((ImTextureID)ENGINE.RES_MNGR->Get<Resources::Texture>("StaticAssets\\Images\\plus.png")->ID,
        { 21,21 }, { 0,1 }, { 1,0 }, 6))
    {
        ImGui::OpenPopup("newComponentList");
    }

    if (ImGui::BeginPopup("newComponentList", NULL)) {

		if (!hasTransform && ImGui::MenuItem("Transform##new"))
		{
			ENGINE.ENT_MNGR->AddComponent<ECS::Components::TransformComponent>(_id);
		}

		if (!hasMeshRenderer && ImGui::MenuItem("MeshRenderer##new"))
		{
			ENGINE.ENT_MNGR->AddComponent<ECS::Components::MeshRendererComponent>(_id);
		}

		if (!hasDirectionalLight && ImGui::MenuItem("DirectionalLight##new"))
		{
			ENGINE.ENT_MNGR->AddComponent<ECS::Components::DirectionalLightComponent>(_id);

		}
		if (!hasPointLight && ImGui::MenuItem("PointLight##new"))
		{
			ENGINE.ENT_MNGR->AddComponent<ECS::Components::PointLightComponent>(_id);

		}
		if (!hasSpotLight && ImGui::MenuItem("SpotLight##new"))
		{
			ENGINE.ENT_MNGR->AddComponent<ECS::Components::SpotLightComponent>(_id);

        }

		if (!hasCamera && ImGui::MenuItem("Camera##new"))
		{
			ENGINE.ENT_MNGR->AddComponent<ECS::Components::CameraComponent>(_id);

        }

		if (!hasAudioSource && ImGui::MenuItem("AudioSource##new"))
		{
			ENGINE.ENT_MNGR->AddComponent<ECS::Components::AudioSourceComponent>(_id);

		}
		if (!hasAudioListener && ImGui::MenuItem("AudioListener##new"))
		{
			ENGINE.ENT_MNGR->AddComponent<ECS::Components::AudioListenerComponent>(_id).Init();

        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();

}