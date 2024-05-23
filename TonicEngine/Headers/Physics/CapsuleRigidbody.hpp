#pragma once
#include <Physics/IRigidbody.hpp>

namespace Physics
{
	class TONIC_ENGINE_API CapsuleRigidbody : public IRigidbody
	{
		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		CapsuleRigidbody() = delete;
		CapsuleRigidbody(double _capsuleRadius, double _capsuleHeight,
			Maths::Vec3 _boxPosition,
			Maths::Quat _boxRotation = Maths::Quat::Identity(),
			Motions::MotionType _motionType = Motions::MotionType::Dynamic,
			bool _bActivateAtCreation = false);
		~CapsuleRigidbody();

		double GetHeight() const;
		double GetRadius() const;
		void SetBounds(double _newRadius, double _newHeight);
	};
}