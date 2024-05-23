#pragma once

#include "ECS/Base/BaseComponent.hpp"
#include "Maths/Maths.hpp"

namespace ECS::Systems
{
	class TransformSystem;
	class BoxRigidbodySystem;
	class SphereRigidbodySystem;
	class CapsuleRigidbodySystem;
}

namespace ECS::Components
{
	struct TONIC_ENGINE_API TransformComponent : public BaseComponent
	{
	protected:
		Maths::Vec3 position_ = (0.f);
		Maths::Quat rotation_ = Maths::Quat::Identity();
		Maths::Vec3 scale_ = (1.f);

		Maths::Vec3 positionDiff_ = (0.f);
		Maths::Quat rotationDiff_ = Maths::Quat::Identity();
		Maths::Vec3 scaleDiff_ = (1.f);

		bool bChanged_ = false;

		friend class ECS::Systems::TransformSystem;
		friend class ECS::Systems::BoxRigidbodySystem;
		friend class ECS::Systems::SphereRigidbodySystem;
		friend class ECS::Systems::CapsuleRigidbodySystem;

	public:
		//Properties
		__declspec(property(get = GetPosition, put = SetPosition))
			Maths::Vec3 position;
		__declspec(property(get = GetRotationFromEuler, put = SetRotationFromEuler))
			Maths::Vec3 eulerDegRotation;
		__declspec(property(get = GetScale, put = SetScale))
			Maths::Vec3 scale;

		__declspec(property(get = GetRotation, put = SetRotation))
			Maths::Quat rotation;

		TransformComponent() = default;
		~TransformComponent() = default;

		TransformComponent(const Maths::Vec3& _position, Maths::Quat _rotation = Maths::Quat::Identity(), Maths::Vec3 _scale = (1.f));
		//In Degrees not radians
		TransformComponent(const Maths::Vec3& _position, Maths::Vec3 _rotationEulerInDeg, Maths::Vec3 _scale = (1.f));

		TransformComponent(const Maths::Vec3& _position, float _rotationAngle, Maths::Vec3 _rotationAxis, Maths::Vec3 _scale = (1.f));

		bool HasChanged()const;

		const Maths::Vec3 GetPosition() const;
		//TODO Uncomment on Child/parent creation
		const Maths::Vec3 GetLocalPosition() const;
		//In Degrees not radians
		const Maths::Vec3 GetRotationFromEuler() const;
		const Maths::Quat GetRotation() const;
		const Maths::Vec3 GetScale() const;
		const Maths::Mat4 GetModel() const;

		void SetPosition(Maths::Vec3 _newPosition);
		void SetLocalPosition(Maths::Vec3 _newPosition);

		//In Degrees not radians
		void SetRotationFromEuler(Maths::Vec3 _newRotation);
		void SetRotation(Maths::Quat _newRotation);
		void SetScale(Maths::Vec3 _newScale);

		void LookAt(Maths::Vec3 _pointOfInterest);

	protected:
		void ResetChanged();
	};
}