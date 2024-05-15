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

Resources::Scene::Scene()
{
}

Resources::Scene::~Scene()
{
	//delete p_entityManager_;
}

void Resources::Scene::Destroy()
{
	return;
	//TODO Destroy the EM
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
	if (!bLoaded_)
	{
		DEBUG_ERROR("SCENE NOT LOADED!")
			return;
	}

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

		struct
		{
			u64 id;
			u64 parrentId;
			u64 sizeOfName;
		}
		data
		{
			entity.first,
			entity.second->GetParent(),
			entity.second->name.size()
		};

		file.write(reinterpret_cast<const char*>(&data), sizeof(data));

		file.write(entity.second->name.c_str(), data.sizeOfName);
	}

	struct
	{
		u64 componentType;
		u64 componentTypeCount;
	} componentHeader{ -1,0 };

	//transform
	{
		// ------------------------------------------------------------------
		// TRANSFORM
		// ------------------------------------------------------------------

		GetComponentHeader< ECS::Components::TransformComponent >(componentHeader.componentType, componentHeader.componentTypeCount);
		file.write(reinterpret_cast<const char*>(&componentHeader), sizeof(componentHeader));

		for (const auto& component : p_em->GetCompList<ECS::Components::TransformComponent>()->GetData())
		{
			struct
			{
				u64 attached_parent;

				Maths::Vec3 position;
				Maths::Quat rotation;
				Maths::Vec3 scale;
			}
			data
			{
				component.GetID(),

				component.position,
				component.rotation,
				component.scale
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(data));
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
			struct
			{
				u64 attached_parent;

				float volume;
				float pitch;
				bool bSpatialized;
				bool bLoop;
				bool bPlayOnStart;
				u64 resourceId;
			}
			data
			{
				component.GetID(),

				component.volume,
				component.pitch,
				component.bSpatialized,
				component.bLoop,
				component.bPlayOnStart,
				component.p_sound->RMID
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(data));
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
			struct
			{
				u64 attached_parent;
			}
			data
			{
				component.GetID()
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(data));
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
			struct
			{
				u64 attached_parent;

				TNCColor color;
				bool bShadow;
				f32 bias;
				f32 strengh;

				f32 inCutOff;
				f32 outCutOff;
				f32 constant;
				f32 linear;
				f32 quadratic;
			}
			data
			{
				component.GetID(),

				component.light.color,
				component.light.bShadow,
				component.light.bias,
				component.light.strength,

				component.light.inCutOff,
				component.light.outCutOff,
				component.light.constant,
				component.light.linear,
				component.light.quadratic
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(data));
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
			struct
			{
				u64 attached_parent;

				TNCColor color;
				bool bShadow;
				f32 bias;
				f32 strengh;

				f32 constant;
				f32 linear;
				f32 quadratic;
			}
			data
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

			file.write(reinterpret_cast<const char*>(&data), sizeof(data));
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
			struct
			{
				u64 attached_parent;

				TNCColor color;
				bool bShadow;
				f32 bias;
				f32 strengh;
			}
			data
			{
				component.GetID(),

				component.light.color,
				component.light.bShadow,
				component.light.bias,
				component.light.strength
			};

			file.write(reinterpret_cast<const char*>(&data), sizeof(data));
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
			struct
			{
				u64 attached_parent;

				u64 resourceId;
				u64 materialCount;
			}
			data
			{
				component.GetID(),

				component.renderer.p_mesh->RMID,
				component.renderer.p_materials.size()
			};
			file.write(reinterpret_cast<const char*>(&data), sizeof(data));

			//save the material in the scene since no file for it
			for (const auto& material : component.renderer.p_materials)
			{
				struct
				{
					s32 type;
					u64 difuseId;
					u64 specular;
					u64 ao;
					u64 normal;
					u64 roughness;
					f32 shininess;
					f32 gammaCorrection;
				}
				materialData
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

				file.write(reinterpret_cast<const char*>(&materialData), sizeof(materialData));
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
			struct
			{
				u64 attached_parent;

				f32 aspect;
				f32 fovY;
				f32 zNear;
				f32 zFar;
				f32 orthoScale;
				bool bPerspectiveMode;
				bool bUsed;
			}
			data
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

			file.write(reinterpret_cast<const char*>(&data), sizeof(data));
		}

		// ------------------------------------------------------------------
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
		if (!entityData.staticData.parrentId)
			continue;
		p_em->ParentEntity(entityData.staticData.id, entityData.staticData.parrentId);
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
			material->type = static_cast<Resources::MaterialType>(materialData.type);
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

	//p_em->AddComponent< ECS::Components::TransformComponent >(0,())

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
	default:
		DEBUG_ERROR("UNKNOW COMPONENT FOUND")
			break;
	}
}