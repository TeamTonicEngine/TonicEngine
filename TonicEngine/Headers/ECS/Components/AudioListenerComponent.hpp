#pragma once
#include "DLL_API.hpp"

#include "ECS/Base/BaseComponent.hpp"
#include "LowRenderer/Cameras/Camera.hpp"
#include "Maths/Maths.hpp"

namespace ECS::Systems { class AudioListenerSystem; }
constexpr u32 INVALID_ID = -1;

namespace ECS::Components
{
	struct TONIC_ENGINE_API AudioListenerComponent : public BaseComponent
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		static u32 s_ids_;

		u32 id_ = INVALID_ID;
	public:
		friend class Systems::AudioListenerSystem;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/

		// Remember to Init after
		AudioListenerComponent() = default;
		// Remember to Init after
		AudioListenerComponent(const AudioListenerComponent&) = default;
		~AudioListenerComponent() = default;

		void Init();

	protected:
		void SetPosition(Maths::Vec3 _position);
		void SetDirection(Maths::Quat _rotation);
		void SetCone(float _innerAngleRad, float _outerAngleRad, float _outerGain);
		void SetVelocity(Maths::Vec3 _velocity);

		void SetEnabled(bool _value) override;
	};
}