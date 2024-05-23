#pragma once
#include "DLL_API.hpp"

#include "IResource.hpp"

#include "ECS/Base/BaseComponent.hpp"
#include "ECS/Base/BaseSystem.hpp"
#include "ECS/Base/CompList.hpp"
#include "ECS/Base/Types.hpp"
#include "ECS/Base/EntityData.hpp"

#include <map>

struct EntitySceneData
{
	struct
	{
		u64 id = (u64)-1;
		u64 parentId = (u64)-1;
		u64 sizeOfName = (u64)-1;
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
		u64 attached_parent = (u64)-1;
		u64 resourceId = 0;
		u64 materialCount = (u64)-1;
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

struct FontSceneData
{
	struct
	{
		u64 attached_parent;
		u64 id;
		TNCColor color;
		u32 priority;
		bool b3D;
		u64 sizeOfText;
	}staticData;

	std::string text;
};

struct ScriptSceneData
{
	struct
	{
		u64 attached_parent;
		u64 sizeOfName;
		u32 type;
	}staticData;

	std::string name;
};

struct RigidBodySceneData : public ComponentData
{
	Maths::Vec3 centerOffset;
	Maths::Quat rotationOffset;
	u32 motionType;
	bool bMovementAtStart;
};

struct BoxRigidBodySceneData : public RigidBodySceneData
{
	Maths::Vec3 extents;
};

struct SphereRigidBodySceneData : public RigidBodySceneData
{
	double radius;
};

struct CapsuleRigidBodySceneData : public SphereRigidBodySceneData
{
	double height;
};

namespace Resources
{
	class Scene : public IResource, public std::enable_shared_from_this<Scene>
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		std::vector< EntitySceneData > entitiesData_;
		std::vector< TransformSceneData > transformsData_;
		std::vector< AudioSourceSceneData > audioSourcesData_;
		std::vector< ComponentData > audioListenersData_;
		std::vector< SpotLightSceneData > spotLightsData_;
		std::vector< PointLightSceneData > pointLightsData_;
		std::vector< DirectionalLightSceneData > directionLightsData_;
		std::vector< MeshRendererSceneData > meshRenderersData_;
		std::vector< CameraSceneData > camerasData_;
		std::vector< FontSceneData > fontData_;
		std::vector< ScriptSceneData > scriptData_;
		
		std::vector< BoxRigidBodySceneData > boxRigidBodyData_;
		std::vector< SphereRigidBodySceneData > sphereRigidBodyData_;
		std::vector< CapsuleRigidBodySceneData > capsuleRigidBodyData_;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/

	public:
		TONIC_ENGINE_API Scene() = default;
		TONIC_ENGINE_API ~Scene() = default;

		void Destroy() override {};

		static void TONIC_ENGINE_API SaveFile(const char* _path);

		void ReadFile(const fs::path _path) override;
		void LoadFile() override {};

		void ResourceUnload() override {};
		void Use() override;

	private:
		void ReadComponent(u64 _componentType, u64 _componentCount, std::ifstream& _file);
	};
	using ScenePtr = std::shared_ptr<Scene>;
}