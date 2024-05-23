#pragma once
#include "DLL_API.hpp"

namespace Physics
{
	namespace Motions
	{
		// Motion type of a physics body
		enum class TONIC_ENGINE_API MotionType : unsigned char
		{
			Static,						// Non movable
			Kinematic,					// Movable using velocities only, does not respond to forces
			Dynamic,					// Responds to forces as a normal physics object
		};

		static std::string MotionTypeToString(MotionType _motionType)
		{
			switch (_motionType)
			{
			case Physics::Motions::MotionType::Static:
				return "Static";
			case Physics::Motions::MotionType::Kinematic:
				return "Kinematic";
			case Physics::Motions::MotionType::Dynamic:
				return "Dynamic";

			default:
				return "ERROR Motion Type Unkown";
			}
		};
	}

	class TONIC_ENGINE_API IRigidbody
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		void* ID = nullptr;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		virtual void SetEnable(bool _bEnable);
		virtual bool GetEnable() const;

		// Will override the current center
		virtual void SetCenter(Maths::Vec3 _newCenter);
		virtual Maths::Vec3 GetCenter() const;

		// Will override the current rotation
		virtual void SetRotation(Maths::Quat _newRotation);
		virtual Maths::Quat GetRotation() const;

		// Will override the current velocity
		virtual void SetLinearVelocity(Maths::Vec3 _newVelocity);
		virtual Maths::Vec3 GetLinearVelocity() const;

		// Will override the current velocity
		virtual void SetAngularVelocity(Maths::Vec3 _newVelocity);
		virtual Maths::Vec3 GetAngularVelocity() const;

		// Will override the current velocity
		virtual void SetAngularAndLinearVelocity(Maths::Vec3 _newAngular, Maths::Vec3 _newLinear);

		virtual void SetMotionType(Motions::MotionType _newMotionType);
		virtual Motions::MotionType GetMotionType() const;

		virtual bool IsActive();

		virtual void RemoveFromPhysic();
		virtual void DestroyBody();
	};
}