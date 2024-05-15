#pragma once
#include "DLL_API.hpp"

#include "IResource.hpp"

#include "ECS/Base/BaseComponent.hpp"
#include "ECS/Base/BaseSystem.hpp"
#include "ECS/Base/CompList.hpp"
#include "ECS/Base/Types.hpp"
#include "ECS/Base/EntityData.hpp"

#include <map>

//namespace ECS
//{
	//class EntityManager;
//}

struct EntitySceneData
{
	struct
	{
		u64 id;
		u64 parrentId;
		u64 sizeOfName;
	}staticData;

	std::string name;
};

struct ComponentData
{
	u64 attached_parent;
};

struct TransformSceneData : public ComponentData
{

	Maths::Vec3 position;
	Maths::Quat rotation;
	Maths::Vec3 scale;
};

struct AudioSourceSceneData : public ComponentData
{
	float volume;
	float pitch;
	bool bSpatialized;
	bool bLoop;
	bool bPlayOnStart;
	u64 resourceId;
};

struct DirectionalLightSceneData : public ComponentData
{
	TNCColor color;
	bool bShadow;
	f32 bias;
	f32 strengh;
};

struct PointLightSceneData : public DirectionalLightSceneData
{
	f32 constant;
	f32 linear;
	f32 quadratic;
};

struct SpotLightSceneData : public PointLightSceneData
{
	f32 inCutOff;
	f32 outCutOff;
};

struct MaterialData
{
	s32 type;
	u64 difuseId;
	u64 specular;
	u64 ao;
	u64 normal;
	u64 roughness;
	f32 shininess;
	f32 gammaCorrection;
};

struct MeshRendererSceneData
{
	struct
	{
		u64 attached_parent;
		u64 resourceId;
		u64 materialCount;
	}staticData;
	std::vector< MaterialData >materials;
};

struct CameraSceneData : public ComponentData
{
	f32 aspect;
	f32 fovY;
	f32 zNear;
	f32 zFar;
	f32 orthoScale;
	bool bPerspectiveMode;
	bool bUsed;
};

namespace Resources
{
	class Scene;
	using ScenePtr = std::shared_ptr<Scene>;
	class Scene : public IResource, public std::enable_shared_from_this<Scene>
	{
	private:
		//std::map < ECS::EntityID, std::shared_ptr<ECS::EntitySignature >> entitiesSignatures_;
		//std::vector<ECS::ICompList> r;

		std::vector< EntitySceneData > entitiesData_;
		std::vector< TransformSceneData > transformsData_;
		std::vector< AudioSourceSceneData > audioSourcesData_;
		std::vector< ComponentData > audioListenersData_;
		std::vector< SpotLightSceneData > spotLightsData_;
		std::vector< PointLightSceneData > pointLightsData_;
		std::vector< DirectionalLightSceneData > directionLightsData_;
		std::vector< MeshRendererSceneData > meshRenderersData_;
		std::vector< CameraSceneData > camerasData_;

	public:
		TONIC_ENGINE_API Scene();
		TONIC_ENGINE_API ~Scene();

		void Destroy() override;

		void TONIC_ENGINE_API SaveFile(const char* _path);

		void ReadFile(const fs::path _path) override;
		void LoadFile() override {};
		void MetaWriteFile(const string _name) override {};
		void MetaReadFile(const string _name) override {};
		void ResourceUnload() override {};
		void Use() override;
	private:
		void ReadComponent(u64 _componentType, u64 _componentCount, std::ifstream& _file);
	};
}