#pragma once
#include "ECS/Components/IRigidbodyComponent.hpp"

namespace ECS::Systems { class BoxRigidbodySystem; }

namespace ECS::Components
{
	struct TONIC_ENGINE_API BoxRigidbodyComponent : public IRigidbodyComponent
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		Maths::Vec3 boxExtents_ = Maths::Vec3(1.f);

	public:
		__declspec(property(get = GetExtents, put = SetExtents))
			Maths::Vec3 extents;

		friend class ECS::Systems::BoxRigidbodySystem;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/

		BoxRigidbodyComponent();
		BoxRigidbodyComponent(Maths::Vec3 _boxExtents,
			Maths::Vec3 _boxPosition,
			Maths::Quat _boxRotation = Maths::Quat::Identity(),
			Physics::Motions::MotionType _motionType = Physics::Motions::MotionType::Dynamic,
			bool _bActivateMovementAtCreation = true);
		~BoxRigidbodyComponent();

		void Init(Maths::Vec3 _size, Maths::Vec3 _position, Maths::Quat _rotation);

		Maths::Vec3 GetExtents() const;
		void SetExtents(Maths::Vec3 _newExtents);
	};
}