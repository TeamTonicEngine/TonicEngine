#pragma once
#include "ECS/Base/BaseComponent.hpp"
namespace ECS::Systems
{
	class CollisionDetectionSystem;
}
namespace ECS::Components
{
	struct IColliderComponent :
		public ECS::BaseComponent
	{
		struct Bounds
		{
			Maths::Vec3 center;
			Maths::Vec3 extents;
			Maths::Vec3 min;
			Maths::Vec3 max;
			Maths::Vec3 size;
		};
	public:
		std::vector<IColliderComponent> overlappingColliders;
		Bounds bounds;
		float contactOffset;
		bool bEnabled;
		bool bisTrigger;

		u8 layer;
		u8 excludeLayers;
		u8 includeLayers;

		friend class ECS::Systems::CollisionDetectionSystem;
	};
}

