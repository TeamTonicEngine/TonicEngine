#pragma once

#include "DLL_API.hpp"

#include "ECS/Base/BaseComponent.hpp"
#include "LowRenderer/Cameras/Camera.hpp"
#include "Maths/Maths.hpp"



namespace ECS::Systems
{
	class AudioListenerSystem;
}
constexpr u32 INVALID_ID = -1;
namespace ECS
{
	namespace Components
	{
		struct TONIC_ENGINE_API AudioListenerComponent : public BaseComponent
		{
			AudioListenerComponent();
			~AudioListenerComponent();
			AudioListenerComponent(const AudioListenerComponent&);

			void Init();
			friend class Systems::AudioListenerSystem;

		protected:
			static u32 s_ids;

			u32 id_ = INVALID_ID;
			void SetPosition(Maths::Vec3 _position);
			void SetDirection(Maths::Quat _rotation);
			void SetCone(float _innerAngleRad, float _outerAngleRad, float _outerGain);
			void SetVelocity(Maths::Vec3 _velocity);
		};
	}
}