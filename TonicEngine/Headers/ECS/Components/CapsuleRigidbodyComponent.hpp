#pragma once
#include "ECS/Components/IRigidbodyComponent.hpp"

namespace ECS::Systems { class CapsuleRigidbodySystem; }

namespace ECS::Components
{
	struct TONIC_ENGINE_API CapsuleRigidbodyComponent : public IRigidbodyComponent
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		double capsuleHeight_ = 1, capsuleRadius_ = 1;

	public:
		__declspec(property(get = GetHeight, put = SetHeight))
			double height;

		__declspec(property(get = GetRadius, put = SetRadius))
			double radius;

		friend class ECS::Systems::CapsuleRigidbodySystem;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/

		CapsuleRigidbodyComponent();
		CapsuleRigidbodyComponent(double _capsuleHeight, double _capsuleRadius,
			Maths::Vec3 _capsulePosition,
			Maths::Quat _capsuleRotation = Maths::Quat::Identity(),
			Physics::Motions::MotionType _motionType = Physics::Motions::MotionType::Dynamic,
			bool _bActivateMovementAtCreation = true);
		~CapsuleRigidbodyComponent();

		void Init(Maths::Vec3 _size, Maths::Vec3 _position, Maths::Quat _rotation);

		double GetHeight() const;
		void SetHeight(double _newHeight);

		double GetRadius() const;
		void SetRadius(double _newRadius);
	};
}