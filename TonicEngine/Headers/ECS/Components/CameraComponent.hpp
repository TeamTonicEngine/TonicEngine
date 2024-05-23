#pragma once
#include "DLL_API.hpp"

#include "ECS/Base/BaseComponent.hpp"

#include "LowRenderer/Cameras/Camera.hpp"

#include "Maths/Maths.hpp"

namespace ECS::Systems { class CameraSystem; }

namespace ECS::Components
{
	struct TONIC_ENGINE_API CameraComponent : public ECS::BaseComponent
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		bool bPrimary_ = true;
		LowRenderer::Cameras::Camera camera_;

	public:
		_declspec(property(get = GetCam, put = SetCam))
			LowRenderer::Cameras::Camera camera;

		friend class ECS::Systems::CameraSystem;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/

		CameraComponent()
			: camera_(ENGINE.WDW->GetScreenSize()[0], ENGINE.WDW->GetScreenSize()[1]) {};
		CameraComponent(LowRenderer::Cameras::Camera& _camera)
			: camera_(_camera) {};
		CameraComponent(const CameraComponent&) = default;
		~CameraComponent() = default;

		operator LowRenderer::Cameras::Camera() const { return camera_; }

		void UseCamera() { camera_.bUsed = true; };
		void StopUseCamera() { camera_.bUsed = false; };

		inline LowRenderer::Cameras::Camera GetCam() const { return camera_; };
		inline void SetCam(LowRenderer::Cameras::Camera _camera) { camera_ = _camera; };
	};
}