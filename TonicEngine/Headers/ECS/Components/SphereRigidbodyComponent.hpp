#pragma once
#include "ECS/Components/IRigidbodyComponent.hpp"

namespace ECS::Systems { class SphereRigidbodySystem; }

namespace ECS::Components
{
	struct TONIC_ENGINE_API SphereRigidbodyComponent : public IRigidbodyComponent
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		double sphereRadius_ = 1;

	public:
		__declspec(property(get = GetRadius, put = SetRadius))
			double radius;

		friend class ECS::Systems::SphereRigidbodySystem;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/

		SphereRigidbodyComponent();
		SphereRigidbodyComponent(double _sphereRadius,
			Maths::Vec3 _spherePosition,
			Maths::Quat _sphereRotation = Maths::Quat::Identity(),
			Physics::Motions::MotionType _motionType = Physics::Motions::MotionType::Dynamic,
			bool _bActivateMovementAtCreation = true);
		~SphereRigidbodyComponent();

		void Init(Maths::Vec3 _size, Maths::Vec3 _position, Maths::Quat _rotation);

		double GetRadius() const;
		void SetRadius(double _newRadius);
	};
}