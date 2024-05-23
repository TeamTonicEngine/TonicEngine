#pragma once
#include <Physics/IRigidbody.hpp>

namespace Physics
{
	class TONIC_ENGINE_API SphereRigidbody : public IRigidbody
	{
		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		SphereRigidbody() = delete;
		SphereRigidbody(double _sphereRadius, Maths::Vec3 _spherePosition,
			Maths::Quat _sphereRotation = Maths::Quat::Identity(),
			Motions::MotionType _motionType = Motions::MotionType::Dynamic,
			bool _bActivateAtCreation = false);

		~SphereRigidbody();

		double GetRadius() const;
		void SetBounds(double _newRadius);
	};
}