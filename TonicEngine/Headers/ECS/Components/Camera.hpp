#pragma once

#include "DLL_API.hpp"

#include "ECS/Base/BaseComponent.hpp"
#include "LowRenderer/Camera.hpp"
#include "Maths/Maths.hpp"

namespace ECS::Systems
{
	class CameraSystem;
}

namespace Components
{
	struct TONIC_ENGINE_API CameraComponent : public ECS::BaseComponent
	{
		CameraComponent() = default;
		~CameraComponent() = default;
		operator LowRenderer::Camera() const { return *p_camera_; }
		CameraComponent(CameraComponent&) = default;
		CameraComponent(LowRenderer::Camera& _camera) :p_camera_(&_camera) {};


		friend class ECS::Systems::CameraSystem;
	protected:
		bool bPrimary = true;
		LowRenderer::Camera* p_camera_;
	};
}