#pragma once

#include "ECS/Base/BaseComponent.hpp"
#include "Maths/Maths.hpp"

namespace Components
{
	struct Transform : public ECS::BaseComponent
	{
		Maths::Vec3 position = (0.f);
		Maths::Quat rotation = Maths::Quat::Identity();
		Maths::Vec3 scale = (1.f);

		TONIC_ENGINE_API Transform() = default;
		TONIC_ENGINE_API ~Transform() = default;

		TONIC_ENGINE_API Transform(const Maths::Vec3& _position, Maths::Quat _rotation = Maths::Quat::Identity(), Maths::Vec3 _scale = (1.f));

		TONIC_ENGINE_API Transform(const Maths::Vec3& _position, Maths::Vec3 _rotationEuler, Maths::Vec3 _scale = (1.f));

		TONIC_ENGINE_API Transform(const Maths::Vec3& _position, float _rotationAngle, Maths::Vec3 _rotationAxis, Maths::Vec3 _scale = (1.f));

		Maths::Mat4 TONIC_ENGINE_API GetModel();
		void TONIC_ENGINE_API SetModelUniform();
	};
}