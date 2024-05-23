#pragma once
#include "PhysicsEngine.hpp"

#include <Jolt/Jolt.h>
// Disable common warnings triggered by Jolt
JPH_SUPPRESS_WARNINGS;

#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>

#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/MotionType.h>

#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <Physics/IRigidbody.hpp>

namespace Physics
{
	using namespace JPH::literals;

	// Layer that objects can be in, determines which other objects it can collide with
	// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
	// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
	// but only if you do collision testing).
	namespace Layers
	{
		static constexpr JPH::ObjectLayer NON_MOVING = 0;
		static constexpr JPH::ObjectLayer MOVING = 1;
		static constexpr JPH::ObjectLayer NUM_LAYERS = 2;

		static JPH::ObjectLayer GetLayerFromMotionType(Motions::MotionType _motion)
		{
			switch (_motion)
			{
			case Motions::MotionType::Dynamic:
			case Motions::MotionType::Kinematic:
				return Layers::MOVING;

			case Motions::MotionType::Static:
			default:
				return Layers::NON_MOVING;
			}
		}
	}

	namespace Motions
	{
		static JPH::EMotionType GetJoltMotionFromMotionType(MotionType _motion)
		{
			switch (_motion)
			{
			case MotionType::Dynamic:
				return JPH::EMotionType::Dynamic;
			case MotionType::Kinematic:
				return JPH::EMotionType::Kinematic;

			case MotionType::Static:
			default:
				return JPH::EMotionType::Static;
			}
		}

		static MotionType GetMotionTypeFromJoltMotion(JPH::EMotionType _motion)
		{
			switch (_motion)
			{
			case JPH::EMotionType::Dynamic:
				return MotionType::Dynamic;
			case JPH::EMotionType::Kinematic:
				return MotionType::Kinematic;

			case JPH::EMotionType::Static:
			default:
				return MotionType::Static;
			}
		}
	}

	// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
	// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
	// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
	// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
	// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
	namespace BroadPhaseLayers
	{
		using namespace JPH;
		static constexpr BroadPhaseLayer NON_MOVING(0);
		static constexpr BroadPhaseLayer MOVING(1);
		static constexpr unsigned NUM_LAYERS(2);
	}

	namespace JoltRequired
	{
		/// Class that determines if two object layers can collide
		class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
		{
		public:
			virtual bool ShouldCollide(JPH::ObjectLayer _object1, JPH::ObjectLayer _object2) const override
			{
				switch (_object1)
				{
				case Layers::NON_MOVING:
					return _object2 == Layers::MOVING; // Non moving only collides with moving
				case Layers::MOVING:
					return true; // Moving collides with everything
				default:
					JPH_ASSERT(false);
					return false;
				}
			}
		};

		// BroadPhaseLayerInterface implementation
		// This defines a mapping between object and broadphase layers.
		class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
		{
		public:
			BPLayerInterfaceImpl()
			{
				// Create a mapping table from object to broad phase layer
				objectToBroadPhase_[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
				objectToBroadPhase_[Layers::MOVING] = BroadPhaseLayers::MOVING;
			}

			virtual unsigned GetNumBroadPhaseLayers() const override
			{
				return BroadPhaseLayers::NUM_LAYERS;
			}

			virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer _layer) const override
			{
				JPH_ASSERT(_layer < Layers::NUM_LAYERS);
				return objectToBroadPhase_[_layer];
			}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
			virtual const char* GetBroadPhaseLayerName(BroadPhaseLayer _layer) const override
			{
				switch ((BroadPhaseLayer::Type)_layer)
				{
				case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
				case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
				default:													JPH_ASSERT(false); return "INVALID";
				}
			}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

		private:
			JPH::BroadPhaseLayer objectToBroadPhase_[Layers::NUM_LAYERS];
		};

		/// Class that determines if an object layer can collide with a broadphase layer
		class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
		{
		public:
			virtual bool ShouldCollide(JPH::ObjectLayer _layer1, JPH::BroadPhaseLayer _layer2) const override
			{
				switch (_layer1)
				{
				case Layers::NON_MOVING:
					return _layer2 == BroadPhaseLayers::MOVING;
				case Layers::MOVING:
					return true;
				default:
					JPH_ASSERT(false);
					return false;
				}
			}
		};

		// A contact listener
		class ContactListener : public JPH::ContactListener
		{
		public:
			// See: ContactListener
			virtual JPH::ValidateResult OnContactValidate(const JPH::Body& _body1, const JPH::Body& _body2, JPH::RVec3Arg _baseOffset, const JPH::CollideShapeResult& _collisionResult) override
			{
				DEBUG_LOG("Contact validate callback between %u and %u", _body1.GetID().GetIndex(), _body2.GetID().GetIndex());

				// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
				return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
			}

			virtual void OnContactAdded(const JPH::Body& _body1, const JPH::Body& _body2, const JPH::ContactManifold& _manifold, JPH::ContactSettings& _ioSettings) override
			{
				DEBUG_LOG("A contact was added between %u and %u", _body1.GetID().GetIndex(), _body2.GetID().GetIndex());
			}

			virtual void OnContactPersisted(const JPH::Body& _body1, const JPH::Body& _body2, const JPH::ContactManifold& _manifold, JPH::ContactSettings& _ioSettings) override
			{
				//DEBUG_LOG("A contact was persisted between %u and %u", _body1.GetID().GetIndex(), _body2.GetID().GetIndex());
			}

			virtual void OnContactRemoved(const JPH::SubShapeIDPair& _subShapePair) override
			{
				DEBUG_LOG("A contact was removed between %u and %u", _subShapePair.GetBody1ID().GetIndex(), _subShapePair.GetBody2ID().GetIndex());
			}
		};

		// A activation listener
		class BodyActivationListener : public JPH::BodyActivationListener
		{
		public:
			virtual void OnBodyActivated(const JPH::BodyID& _bodyID, JPH::uint64 _bodyUserData) override
			{
				DEBUG_LOG("A body got activated, ID: %u", _bodyID.GetIndex());
			}

			virtual void OnBodyDeactivated(const JPH::BodyID& _bodyID, JPH::uint64 _bodyUserData) override
			{
				DEBUG_LOG("A body went to sleep, ID: %u", _bodyID.GetIndex());
			}
		};
	}

	class JoltWrapper : public PhysicsEngine
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		JPH::TempAllocatorImpl* p_tempAllocator_ = nullptr;
		JPH::JobSystemThreadPool* p_jobSystem_ = nullptr;
		JPH::PhysicsSystem* p_physicsSystem_ = nullptr;

		JoltRequired::BPLayerInterfaceImpl p_broadPhaseLayerInterface_;
		JoltRequired::ObjectVsBroadPhaseLayerFilterImpl p_objectVsBroadphaseLayerFilter_;
		JoltRequired::ObjectLayerPairFilterImpl p_objectVsObjectLayerFilter_;

		JoltRequired::BodyActivationListener bodyActivationListener_;
		JoltRequired::ContactListener contactListener_;

		int nbBodiesLastCheck = -1;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		const bool Init() override;
		void Update(float _fixedDeltaTime) override;
		void* GetBodyInterface() override;
		void Destroy() override;
	};
}