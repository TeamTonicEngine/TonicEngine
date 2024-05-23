#include "pch.hpp"

#include "Resources/Scene.hpp"

#include "ECS/Base/EntityManager.hpp"
#include "ECS/Systems.hpp"

#include "ECS/Components.hpp"

template<typename T>
inline void GetComponentHeader(u64& _typeId, u64& _count)
{
	_typeId++;
	_count = ENGINE.ENT_MNGR->GetCompList<T>()->GetData().size();
}

void Resources::Scene::ReadFile(const fs::path _path)
{
	std::ifstream file(_path, std::ios::binary);
	if (!file.is_open())
	{
		DEBUG_ERROR("COULD NOT OPEN %s", _path.string().c_str())
			return;
	}

	u64 entityCount;

	file.read(reinterpret_cast<char*>(&entityCount), sizeof(u64));

	for (u64 i = 0; i < entityCount; i++)
	{
		EntitySceneData data;

		file.read(reinterpret_cast<char*>(&data.staticData), sizeof(EntitySceneData::staticData));

		char* name = new char[data.staticData.sizeOfName + 1];
		file.read(name, data.staticData.sizeOfName);
		name[data.staticData.sizeOfName] = '\0';

		data.name = name;

		delete[] name;

		entitiesData_.push_back(data);
	}

	struct
	{
		u64 componentType;
		u64 componentTypeCount;
	}componentHeader;

	while (file.read(reinterpret_cast<char*>(&componentHeader), sizeof(componentHeader)))
	{
		ReadComponent(componentHeader.componentType, componentHeader.componentTypeCount, file);
	}

	file.close();

	bRead_ = bLoaded_ = true;
	return;
}

void Resources::Scene::SaveFile(const char* _path)
{
	//security check to create the directories if dont exist
	fs::path directory = fs::path(_path).parent_path();

	if (!fs::exists(directory)) {
		fs::create_directories(directory);
	}

	std::ofstream file(_path, std::ios::binary);
	if (!file.is_open())
	{
		DEBUG_ERROR("COULD NOT OPEN %s", _path)
			return;
	}

	ECS::EntityManager* p_em = ENGINE.ENT_MNGR;

	u64 entityCount = p_em->GetEntities().size() - 1;

	file.write(reinterpret_cast<const char*>(&entityCount), sizeof(u64));

	for (const auto& entity : p_em->GetEntities())
	{
		if (entity.first == 0) continue;

		EntitySceneData data
		{
			{
				entity.first,
				entity.second->GetParent(),
				entity.second->name.size()
			},
			entity.second->name
		};

		file.write(reinterpret_cast<const char*>(&data.staticData), sizeof(EntitySceneData::staticData));

		file.write(data.name.c_str(), data.staticData.sizeOfName);
	}

	struct
	{
		u64 componentType;
		u64 componentTypeCount;
	} componentHeader{ (u64)-1,0 };

	//transform
	{
		// ------------------------------------------------------------------
		// TRANSFORM
		// ------------------------------------------------------------------

		GetComponentHeader< ECS::Components::TransformComponent >(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::TransformComponent>()->GetData())
		{
			TransformSceneData data
			{
				component.GetID(),

				component.position,
				component.rotation,
				component.scale
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(TransformSceneData));
		}

		// ------------------------------------------------------------------
	}

	//audio source
	{
		// ------------------------------------------------------------------
		// AUDIO SOURCE
		// ------------------------------------------------------------------

		GetComponentHeader< ECS::Components::AudioSourceComponent >(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::AudioSourceComponent>()->GetData())
		{
			AudioSourceSceneData data
			{
				component.GetID(),

				component.volume,
				component.pitch,
				component.bSpatialized,
				component.bLoop,
				component.bPlayOnStart,
				component.p_sound->RMID
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(AudioSourceSceneData));
		}

		// ------------------------------------------------------------------
	}

	//audio listener
	{
		// ------------------------------------------------------------------
		// AUDIO LISTENER
		// ------------------------------------------------------------------

		GetComponentHeader< ECS::Components::AudioListenerComponent >(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::AudioListenerComponent>()->GetData())
		{
			ComponentData data
			{
				component.GetID()
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(ComponentData));
		}

		// ------------------------------------------------------------------
	}

	//spot light
	{
		// ------------------------------------------------------------------
		// SPOT LIGHT
		// ------------------------------------------------------------------

		GetComponentHeader< ECS::Components::SpotLightComponent >(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::SpotLightComponent>()->GetData())
		{

			SpotLightSceneData data
			{
				component.GetID(),

				component.light.color,
				component.light.bShadow,
				component.light.bias,
				component.light.strength,

				component.light.linear,
				component.light.quadratic,

				component.light.inCutOff,
				component.light.outCutOff,
				component.light.constant
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(SpotLightSceneData));
		}

		// ------------------------------------------------------------------
	}

	//point light
	{
		// ------------------------------------------------------------------
		// POINT LIGHT
		// ------------------------------------------------------------------

		GetComponentHeader< ECS::Components::PointLightComponent >(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::PointLightComponent>()->GetData())
		{
			PointLightSceneData data
			{
				component.GetID(),

				component.light.color,
				component.light.bShadow,
				component.light.bias,
				component.light.strength,

				component.light.constant,
				component.light.linear,
				component.light.quadratic
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(PointLightSceneData));
		}

		// ------------------------------------------------------------------
	}

	//directional light
	{
		// ------------------------------------------------------------------
		// DIRECTIONAL LIGHT
		// ------------------------------------------------------------------

		GetComponentHeader< ECS::Components::DirectionalLightComponent >(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::DirectionalLightComponent>()->GetData())
		{
			DirectionalLightSceneData data
			{
				component.GetID(),

				component.light.color,
				component.light.bShadow,
				component.light.bias,
				component.light.strength
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(DirectionalLightSceneData));
		}

		// ------------------------------------------------------------------
	}

	//mesh renderer
	{
		// ------------------------------------------------------------------
		// MESH RENDERER
		// ------------------------------------------------------------------

		GetComponentHeader< ECS::Components::MeshRendererComponent >(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::MeshRendererComponent>()->GetData())
		{
			MeshRendererSceneData data
			{
				{
					component.GetID(),

					component.renderer.p_mesh->RMID,
					component.renderer.p_materials.size()
				},
				{}
			};
			file.write(reinterpret_cast<const char*>(&data.staticData), sizeof(MeshRendererSceneData::staticData));

			//save the material in the scene since no file for it
			for (const auto& material : component.renderer.p_materials)
			{
				MaterialData materialData
				{
					static_cast<s32>(material->type),
					material->diffuse ? material->diffuse->RMID : 0,
					material->specular ? material->specular->RMID : 0,
					material->ao ? material->ao->RMID : 0,
					material->normal ? material->normal->RMID : 0,
					material->roughness ? material->roughness->RMID : 0,
					material->shininess,
					material->gammaCorrection
				};

				file.write(reinterpret_cast<const char*>(&materialData), sizeof(MaterialData));
			}
		}

		// ------------------------------------------------------------------
	}

	//camera
	{
		// ------------------------------------------------------------------
		// CAMERA
		// ------------------------------------------------------------------

		GetComponentHeader< ECS::Components::CameraComponent >(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::CameraComponent>()->GetData())
		{
			CameraSceneData data
			{
				component.GetID(),

				component.camera.aspect,
				component.camera.fovY,
				component.camera.zNear,
				component.camera.zFar,
				component.camera.orthoScale,
				component.camera.bPerspectiveMode,
				component.camera.bUsed
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(CameraSceneData));
		}

		// ------------------------------------------------------------------
	}

	//font
	{
		// ------------------------------------------------------------------
		// FONT
		// ------------------------------------------------------------------

		GetComponentHeader < ECS::Components::TextRendererComponent>(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::TextRendererComponent>()->GetData())
		{
			FontSceneData data
			{
				{
					component.GetID(),

					component.p_font->RMID,
					component.color,
					component.priority,
					component.b3D,
					component.text.size()
				},
				component.text
			};

			file.write(reinterpret_cast<const char*>(&data.staticData), sizeof(FontSceneData::staticData));
			file.write(data.text.c_str(), data.staticData.sizeOfText);
		}
	}

	//script
	{
		// ------------------------------------------------------------------
		// SCRIPT
		// ------------------------------------------------------------------

		GetComponentHeader < ECS::Components::CppScriptComponent>(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::CppScriptComponent>()->GetData())
		{
			ScriptSceneData data
			{
				{
					component.GetID(),
					component.scriptTypeStr.size(),
					(u32)component.scriptType
				},
				component.scriptTypeStr
			};

			file.write(reinterpret_cast<const char*>(&data.staticData), sizeof(ScriptSceneData::staticData));
			file.write(data.name.c_str(), data.staticData.sizeOfName);
		}
	}

	//boxRigidBodyScene
	{
		// ------------------------------------------------------------------
		// BOX RIGID BODY SCENE 
		// ------------------------------------------------------------------

		GetComponentHeader < ECS::Components::BoxRigidbodyComponent>(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::BoxRigidbodyComponent>()->GetData())
		{
			BoxRigidBodySceneData data
			{
				component.GetID(),

				component.centerOffset,
				component.rotationOffset,
				(u32)component.motionType,
				component.bMovementAtStart,

				component.extents
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(BoxRigidBodySceneData));
		}
	}

	//sphereRigidBodyScene
	{
		// ------------------------------------------------------------------
		// SPHERE RIGID BODY SCENE 
		// ------------------------------------------------------------------

		GetComponentHeader < ECS::Components::SphereRigidbodyComponent>(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::SphereRigidbodyComponent>()->GetData())
		{
			SphereRigidBodySceneData data
			{
				component.GetID(),

				component.centerOffset,
				component.rotationOffset,
				(u32)component.motionType,
				component.bMovementAtStart,

				component.radius
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(SphereRigidBodySceneData));
		}
	}

	//capsuleRigidBodyScene
	{
		// ------------------------------------------------------------------
		// CAPSULE RIGID BODY SCENE 
		// ------------------------------------------------------------------

		GetComponentHeader < ECS::Components::CapsuleRigidbodyComponent>(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::CapsuleRigidbodyComponent>()->GetData())
		{
			CapsuleRigidBodySceneData data
			{
				component.GetID(),

				component.centerOffset,
				component.rotationOffset,
				(u32)component.motionType,
				component.bMovementAtStart,

				component.radius,

				component.height
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(CapsuleRigidBodySceneData));
		}
	}

 	file.close();

	return;
}

void Resources::Scene::Use()
{
	if (!bLoaded_)
	{
		DEBUG_ERROR("SCENE NOT LOADED!")
			return;
	}

	ECS::EntityManager* p_em = ENGINE.ENT_MNGR;

	p_em->DestroyAllEntitiesFromEntity();

	p_em->ResetAvailableEntities();

	for (const auto& entityData : entitiesData_)
	{
		p_em->AddEntityWithId(entityData.staticData.id);
		p_em->GetEntityData(entityData.staticData.id)->SetName(entityData.name);
	}
	for (const auto& entityData : entitiesData_)
	{
		if (!entityData.staticData.parentId)
			continue;
		p_em->ParentEntity(entityData.staticData.id, entityData.staticData.parentId);
	}

	//transform
	for (const auto& transformData : transformsData_)
	{
		ECS::Components::TransformComponent temp(transformData.position, transformData.rotation, transformData.scale);

		p_em->AddComponent< ECS::Components::TransformComponent >(transformData.attached_parent, temp);
	}

	//audioSource
	for (const auto& audioSourceData : audioSourcesData_)
	{
		ECS::Components::AudioSourceComponent temp(ENGINE.RES_MNGR->Get<Resources::Sound>(audioSourceData.resourceId));

		temp.volume = audioSourceData.volume;
		temp.pitch = audioSourceData.pitch;
		temp.bSpatialized = audioSourceData.bSpatialized;
		temp.bLoop = audioSourceData.bLoop;
		temp.bPlayOnStart = audioSourceData.bPlayOnStart;

		p_em->AddComponent< ECS::Components::AudioSourceComponent >(audioSourceData.attached_parent, temp);
	}

	//audioListener
	for (const auto& audioListenerData : audioListenersData_)
	{
		ECS::Components::AudioListenerComponent temp;

		p_em->AddComponent< ECS::Components::AudioListenerComponent >(audioListenerData.attached_parent, temp);
	}

	//spotLight
	for (const auto& spotLightData : spotLightsData_)
	{
		LowRenderer::Lights::SpotLight light;

		light.color = spotLightData.color;
		light.bShadow = spotLightData.bShadow;
		light.bias = spotLightData.bias;
		light.strength = spotLightData.strengh;

		light.inCutOff = spotLightData.inCutOff;
		light.outCutOff = spotLightData.outCutOff;

		light.constant = spotLightData.constant;
		light.linear = spotLightData.linear;
		light.quadratic = spotLightData.quadratic;

		ECS::Components::SpotLightComponent temp(light);

		p_em->AddComponent< ECS::Components::SpotLightComponent >(spotLightData.attached_parent, temp);
	}

	//pointLight
	for (const auto& pointLightData : pointLightsData_)
	{
		LowRenderer::Lights::PointLight light;

		light.color = pointLightData.color;
		light.bShadow = pointLightData.bShadow;
		light.bias = pointLightData.bias;
		light.strength = pointLightData.strengh;

		light.constant = pointLightData.constant;
		light.linear = pointLightData.linear;
		light.quadratic = pointLightData.quadratic;

		ECS::Components::PointLightComponent temp(light);

		p_em->AddComponent< ECS::Components::PointLightComponent >(pointLightData.attached_parent, temp);
	}

	//directionLight
	for (const auto& directionLightData : directionLightsData_)
	{
		LowRenderer::Lights::DirectionalLight light;

		light.color = directionLightData.color;
		light.bShadow = directionLightData.bShadow;
		light.bias = directionLightData.bias;
		light.strength = directionLightData.strengh;

		ECS::Components::DirectionalLightComponent temp(light);

		p_em->AddComponent< ECS::Components::DirectionalLightComponent >(directionLightData.attached_parent, temp);
	}

	//meshRenderer
	for (const auto& meshRendererData : meshRenderersData_)
	{
		ECS::Components::MeshRendererComponent temp;

		temp.renderer.p_mesh = ENGINE.RES_MNGR->Get<Resources::Mesh>(meshRendererData.staticData.resourceId);

		for (const auto& materialData : meshRendererData.materials)
		{
			temp.renderer.p_materials.push_back(std::make_shared<Resources::Material>());
			const auto& material = temp.renderer.p_materials.back();
			material->type = static_cast<Resources::Materials::MaterialType>(materialData.type);
			material->diffuse = ENGINE.RES_MNGR->Get<Resources::Texture>(materialData.difuseId);
			material->specular = ENGINE.RES_MNGR->Get<Resources::Texture>(materialData.specular);
			material->ao = ENGINE.RES_MNGR->Get<Resources::Texture>(materialData.ao);
			material->normal = ENGINE.RES_MNGR->Get<Resources::Texture>(materialData.normal);
			material->roughness = ENGINE.RES_MNGR->Get<Resources::Texture>(materialData.roughness);
			material->shininess = materialData.shininess;
			material->gammaCorrection = materialData.gammaCorrection;
		}

		p_em->AddComponent< ECS::Components::MeshRendererComponent >(meshRendererData.staticData.attached_parent, temp);
	}

	//camera
	for (const auto& cameraData : camerasData_)
	{
		ECS::Components::CameraComponent temp;
		LowRenderer::Cameras::Camera camera = temp.camera;

		camera.aspect = cameraData.aspect;
		camera.fovY = cameraData.fovY;
		camera.zNear = cameraData.zNear;
		camera.zFar = cameraData.zFar;
		camera.orthoScale = cameraData.orthoScale;
		camera.bPerspectiveMode = cameraData.bPerspectiveMode;
		camera.bUsed = cameraData.bUsed;;

		temp.camera = camera;
		p_em->AddComponent< ECS::Components::CameraComponent >(cameraData.attached_parent, temp);
	}

	//font
	for (const auto& fontData : fontData_)
	{
		ECS::Components::TextRendererComponent temp;

		temp.p_font = ENGINE.RES_MNGR->Get<Resources::Font>(fontData.staticData.id);
		temp.color = fontData.staticData.color;
		temp.priority = fontData.staticData.priority;
		temp.b3D = fontData.staticData.b3D;
		temp.text = fontData.text;

		p_em->AddComponent< ECS::Components::TextRendererComponent >(fontData.staticData.attached_parent, temp);
	}

	//script
	for (const auto& scriptData : scriptData_)
	{
		auto& t = p_em->AddComponent< ECS::Components::CppScriptComponent >(scriptData.staticData.attached_parent, ECS::Components::CppScriptComponent());
		t.Bind((ECS::ScriptType)scriptData.staticData.type);
		t.scriptTypeStr = scriptData.name;
	}

	//boxRigidBody
	for (const auto& boxRigidBodyData : boxRigidBodyData_)
	{
		auto& temp = p_em->AddComponent< ECS::Components::BoxRigidbodyComponent >(boxRigidBodyData.attached_parent, ECS::Components::BoxRigidbodyComponent());
		temp.centerOffset = boxRigidBodyData.centerOffset;
		temp.rotationOffset = boxRigidBodyData.rotationOffset;
		temp.motionType = (Physics::Motions::MotionType)boxRigidBodyData.motionType;
		temp.bMovementAtStart = boxRigidBodyData.bMovementAtStart;
		temp.extents = boxRigidBodyData.extents;
	}

	//sphereRigidBody
	for (const auto& sphereRigidBodyData : sphereRigidBodyData_)
	{
		auto& temp = p_em->AddComponent< ECS::Components::SphereRigidbodyComponent >(sphereRigidBodyData.attached_parent, ECS::Components::SphereRigidbodyComponent());
		temp.centerOffset = sphereRigidBodyData.centerOffset;
		temp.rotationOffset = sphereRigidBodyData.rotationOffset;
		temp.motionType = (Physics::Motions::MotionType)sphereRigidBodyData.motionType;
		temp.bMovementAtStart = sphereRigidBodyData.bMovementAtStart;
		temp.radius = sphereRigidBodyData.radius;
	}

	//capsuleRigidBody
	for (const auto& capsuleRigidBodyData : capsuleRigidBodyData_)
	{
		auto& temp = p_em->AddComponent< ECS::Components::CapsuleRigidbodyComponent >(capsuleRigidBodyData.attached_parent, ECS::Components::CapsuleRigidbodyComponent());
		temp.centerOffset = capsuleRigidBodyData.centerOffset;
		temp.rotationOffset = capsuleRigidBodyData.rotationOffset;
		temp.motionType = (Physics::Motions::MotionType)capsuleRigidBodyData.motionType;
		temp.bMovementAtStart = capsuleRigidBodyData.bMovementAtStart;
		temp.radius = capsuleRigidBodyData.radius;
		temp.height = capsuleRigidBodyData.height;
	}

	return;
}

void Resources::Scene::ReadComponent(u64 _componentType, u64 _componentCount, std::ifstream& _file)
{
	switch (_componentType)
	{
	case 0: //transform
	{
		for (u64 i = 0; i < _componentCount; i++)
		{
			TransformSceneData data;

			_file.read(reinterpret_cast<char*>(&data), sizeof(TransformSceneData));

			transformsData_.push_back(data);
		}
		break;
	}
	case 1: //audio source
	{
		for (u64 i = 0; i < _componentCount; i++)
		{
			AudioSourceSceneData data;

			_file.read(reinterpret_cast<char*>(&data), sizeof(AudioSourceSceneData));

			audioSourcesData_.push_back(data);
		}
		break;
	}
	case 2: //audio listener
	{
		for (u64 i = 0; i < _componentCount; i++)
		{
			ComponentData data;

			_file.read(reinterpret_cast<char*>(&data), sizeof(ComponentData));

			audioListenersData_.push_back(data);
		}
		break;
	}
	case 3: //spot light
	{
		for (u64 i = 0; i < _componentCount; i++)
		{
			SpotLightSceneData data;

			_file.read(reinterpret_cast<char*>(&data), sizeof(SpotLightSceneData));

			spotLightsData_.push_back(data);
		}
		break;
	}
	case 4: //point light
	{
		for (u64 i = 0; i < _componentCount; i++)
		{
			PointLightSceneData data;

			_file.read(reinterpret_cast<char*>(&data), sizeof(PointLightSceneData));

			pointLightsData_.push_back(data);
		}
		break;
	}
	case 5: //directional light
	{
		for (u64 i = 0; i < _componentCount; i++)
		{
			DirectionalLightSceneData data;

			_file.read(reinterpret_cast<char*>(&data), sizeof(DirectionalLightSceneData));

			directionLightsData_.push_back(data);
		}
		break;
	}
	case 6: //mesh renderer
	{
		for (u64 i = 0; i < _componentCount; i++)
		{
			MeshRendererSceneData data;

			_file.read(reinterpret_cast<char*>(&data.staticData), sizeof(MeshRendererSceneData::staticData));

			for (u64 j = 0; j < data.staticData.materialCount; j++)
			{
				MaterialData materialData;

				_file.read(reinterpret_cast<char*>(&materialData), sizeof(MaterialData));

				data.materials.push_back(materialData);
			}

			meshRenderersData_.push_back(data);
		}
		break;
	}
	case 7: //camera
	{
		for (u64 i = 0; i < _componentCount; i++)
		{
			CameraSceneData data;

			_file.read(reinterpret_cast<char*>(&data), sizeof(CameraSceneData));

			camerasData_.push_back(data);
		}
		break;
	}
	case 8: //camera
	{
		for (u64 i = 0; i < _componentCount; i++)
		{
			FontSceneData data;

			_file.read(reinterpret_cast<char*>(&data.staticData), sizeof(FontSceneData::staticData));

			char* name = new char[data.staticData.sizeOfText + 1];
			_file.read(name, data.staticData.sizeOfText);
			name[data.staticData.sizeOfText] = '\0';

			data.text = name;

			delete[] name;

			fontData_.push_back(data);
		}
		break;
	}
	case 9: //script
	{
		for (u64 i = 0; i < _componentCount; i++)
		{
			ScriptSceneData data;

			_file.read(reinterpret_cast<char*>(&data.staticData), sizeof(ScriptSceneData::staticData));

			char* name = new char[data.staticData.sizeOfName + 1];
			_file.read(name, data.staticData.sizeOfName);
			name[data.staticData.sizeOfName] = '\0';

			data.name = name;

			delete[] name;

			scriptData_.push_back(data);
		}
		break;
	}
	case 10: //boxRigidBody
	{
		for (u64 i = 0; i < _componentCount; i++)
		{
			BoxRigidBodySceneData data;

			_file.read(reinterpret_cast<char*>(&data), sizeof(BoxRigidBodySceneData));

			boxRigidBodyData_.push_back(data);
		}
		break;
	}
	case 11: //sphereRigidBody
	{
		for (u64 i = 0; i < _componentCount; i++)
		{
			SphereRigidBodySceneData data;

			_file.read(reinterpret_cast<char*>(&data), sizeof(SphereRigidBodySceneData));

			sphereRigidBodyData_.push_back(data);
		}
		break;
	}
	case 12: //capsuleRigidBody
	{
		for (u64 i = 0; i < _componentCount; i++)
		{
			CapsuleRigidBodySceneData data;

			_file.read(reinterpret_cast<char*>(&data), sizeof(CapsuleRigidBodySceneData));

			capsuleRigidBodyData_.push_back(data);
		}
		break;
	}
	default:
		DEBUG_ERROR("UNKNOW COMPONENT FOUND")
			break;
	}
}