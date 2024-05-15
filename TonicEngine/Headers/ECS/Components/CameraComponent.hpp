#pragma once

#include "DLL_API.hpp"

#include "ECS/Base/BaseComponent.hpp"
#include "LowRenderer/Cameras/Camera.hpp"
#include "Maths/Maths.hpp"

namespace ECS::Systems
{
	class CameraSystem;
}

namespace ECS::Components
{
	struct TONIC_ENGINE_API CameraComponent : public ECS::BaseComponent
	{
		CameraComponent() : camera_(ENGINE.WDW->GetScreenSize()[0], ENGINE.WDW->GetScreenSize()[1]) {};
		~CameraComponent() = default;
		operator LowRenderer::Cameras::Camera() const { return camera_; }
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(LowRenderer::Cameras::Camera& _camera) : camera_(_camera) {};

		void UseCamera() { camera_.bUsed = true; };
		void StopUseCamera() { camera_.bUsed = false; };

		friend class ECS::Systems::CameraSystem;

		_declspec(property(get = GetCam, put = SetCam))
			LowRenderer::Cameras::Camera camera;
		inline LowRenderer::Cameras::Camera GetCam() const { return camera_; };
		inline void SetCam(LowRenderer::Cameras::Camera _camera) { camera_ = _camera; };

	protected:
		bool bPrimary = true;
		LowRenderer::Cameras::Camera camera_;
	};
}