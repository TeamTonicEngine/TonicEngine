#pragma once
#include <Physics/IRigidbody.hpp>

namespace Physics
{
	class TONIC_ENGINE_API BoxRigidbody : public IRigidbody
	{
		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		BoxRigidbody() = delete;
		BoxRigidbody(Maths::Vec3 _boxExtents, Maths::Vec3 _boxPosition,
			Maths::Quat _boxRotation = Maths::Quat::Identity(),
			Motions::MotionType _motionType = Motions::MotionType::Dynamic,
			bool _bActivateAtCreation = false);
		~BoxRigidbody();

		Maths::Vec3 GetBounds() const;
		void SetBounds(Maths::Vec3 _newBounds);
	};
}