#pragma once
#include "DLL_API.hpp"

#include "ECS/Base/BaseComponent.hpp"

#include <Physics/IRigidbody.hpp>

namespace ECS::Systems { class CollisionDetectionSystem; }

namespace ECS::Components
{
	struct IRigidbodyComponent : public ECS::BaseComponent
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		std::shared_ptr<Physics::IRigidbody> p_rigidbody_;

		// Offsets
		Maths::Vec3 center_ = Maths::Vec3(0.f);
		Maths::Vec3 centerOffset_ = Maths::Vec3(0.f);
		Maths::Quat rotation_ = Maths::Quat::Identity();
		Maths::Quat rotationOffset_ = Maths::Quat::Identity();

		Physics::Motions::MotionType motionType_ = Physics::Motions::MotionType::Dynamic;
		bool bMovementAtStart_ = true;

	public:
		// This enables the simuation on a moving body (i.e. a static body will always be off)
		__declspec(property(get = IsMovementEnabled, put = SetMovementEnabled))
			bool bMovementEnabled;

		// This the true position of the rigidbody
		__declspec(property(get = GetPosition))
			Maths::Vec3 position;

		// This the center of the rigidbody aka the position of the transform component
		__declspec(property(get = GetCenter, put = SetCenter)) // This is modified by the transform in the system
			Maths::Vec3 center;

		// This the center offset of the rigidbody aka the position of the rigidbody component
		__declspec(property(get = GetCenterOffset, put = SetCenterOffset)) // This is modified in the editor
			Maths::Vec3 centerOffset;

		// This the true rotation of the rigidbody
		__declspec(property(get = GetRotation))
			Maths::Quat rotation;

		// This the rotation of the rigidbody aka the rotation of the transform component
		__declspec(property(get = GetRotationParent, put = SetRotationParent)) // This is modified by the transform in the system
			Maths::Quat rotationParent;

		// This the rotation offset of the rigidbody aka the rotation of the rigidbody component
		__declspec(property(get = GetRotationOffset, put = SetRotationOffset)) // This is modified in the editor
			Maths::Quat rotationOffset;

		__declspec(property(get = GetMotionType, put = SetMotionType))
			Physics::Motions::MotionType motionType;

		__declspec(property(get = GetAngularVelocity, put = SetAngularVelocity))
			Maths::Vec3 angularVelocity;

		__declspec(property(get = GetLinearVelocity, put = SetLinearVelocity))
			Maths::Vec3 linearVelocity;

		__declspec(property(get = GetMovementAtStart, put = SetMovementAtStart))
			bool bMovementAtStart;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/

		virtual TONIC_ENGINE_API ~IRigidbodyComponent() = default;

		const bool TONIC_ENGINE_API RigidbodyExist() { return p_rigidbody_.get(); }

		void UpdateRigidbodyCenter() { p_rigidbody_->SetCenter(center_ + centerOffset_); }
		void UpdateRigidbodyRotation() { p_rigidbody_->SetRotation(rotationOffset_ * rotation_); }

		void DestroyRigidbody() { if (p_rigidbody_) p_rigidbody_ = nullptr; }

		bool IsMovementEnabled() const { return p_rigidbody_->GetEnable(); };
		void SetMovementEnabled(bool _bMovementEnabled) { p_rigidbody_->SetEnable(_bMovementEnabled); };

		Maths::Vec3 GetPosition() const { return p_rigidbody_->GetCenter(); };

		Maths::Vec3 GetCenter() const { return center_; };
		void SetCenter(Maths::Vec3 _newCenter) { center_ = _newCenter; UpdateRigidbodyCenter(); };

		Maths::Vec3 GetCenterOffset() const { return centerOffset_; };
		void SetCenterOffset(Maths::Vec3 _newCenterOffset) { centerOffset_ = _newCenterOffset; UpdateRigidbodyCenter(); };

		Maths::Quat GetRotation() const { return p_rigidbody_->GetRotation(); };

		Maths::Quat GetRotationParent() const { return rotation_; };
		void SetRotationParent(Maths::Quat _newRotationParent) { rotation_ = _newRotationParent;  UpdateRigidbodyRotation(); };

		Maths::Quat GetRotationOffset() const { return rotationOffset_; };
		void SetRotationOffset(Maths::Quat _newRotationOffset) { rotationOffset_ = _newRotationOffset; UpdateRigidbodyRotation(); };

		Physics::Motions::MotionType GetMotionType() const { return p_rigidbody_->GetMotionType(); };
		void SetMotionType(Physics::Motions::MotionType _newMotionType) { p_rigidbody_->SetMotionType(_newMotionType); };

		Maths::Vec3 GetAngularVelocity() const { return p_rigidbody_->GetAngularVelocity(); };
		void SetAngularVelocity(Maths::Vec3 _newAngularVelocity) { p_rigidbody_->SetAngularVelocity(_newAngularVelocity); };

		Maths::Vec3 GetLinearVelocity() const { return p_rigidbody_->GetLinearVelocity(); };
		void SetLinearVelocity(Maths::Vec3 _newLinearVelocity) { p_rigidbody_->SetLinearVelocity(_newLinearVelocity); };

		bool GetMovementAtStart() const { return bMovementAtStart_; };
		void SetMovementAtStart(bool _state) { bMovementAtStart_ = _state; };
	};
}