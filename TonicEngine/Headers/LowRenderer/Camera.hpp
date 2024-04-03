#pragma once

#include "Maths/Maths.hpp"

#include <DLL_API.hpp>

namespace LowRenderer::Cameras
{
	struct TONIC_ENGINE_API CameraInput
	{
		float deltaX;
		float deltaY;
		float zoom;
		bool bMoveForward;
		bool bMoveBackward;
		bool bMoveUp;
		bool bMoveDown;
		bool bMoveLeft;
		bool bMoveRight;
		// ... and more if needed
		bool NoInputs() const;
	};

	struct TONIC_ENGINE_API Input
	{
		//Only for scene vizualizer Camera
		CameraInput cameraInput{};
		struct Mouse
		{
			double x = 0.0;
			double y = 0.0;
			float deltaX = 0.f;
			float deltaY = 0.f;
			static float s_scrollOffset;
		};
		Mouse mouse;
	};

	class Camera
	{
	public:
		//Required only for active camera
		Maths::Mat4 view;
		Maths::Mat4 projection;
		Maths::Mat4 viewProjection;

		unsigned int width, height;
		float aspect;
		float fovY;
		float zNear, zFar;
		float orthoScale = 2.f;

		bool bViewChanged;
		bool bPerspectiveMode = true;
		bool bProjChanged;
		bool bUsed = false;

		TONIC_ENGINE_API Camera(unsigned int _width, unsigned int _height);

		void TONIC_ENGINE_API Update(Maths::Vec3 _position, Maths::Quat _rotation);
		void TONIC_ENGINE_API SetView(Maths::Vec3 _position, Maths::Vec3 _forward, Maths::Vec3 _up);
		void TONIC_ENGINE_API SetView(Maths::Vec3 _position, Maths::Quat _rotation);
		void TONIC_ENGINE_API SetProjection();
		void TONIC_ENGINE_API ComputeViewProjection();
		//void TONIC_ENGINE_API ShowImGuiControls();

	protected:
		Maths::Mat4 Frustum(float _left, float _right, float _bottom, float _top, float _near, float _far);
		Maths::Mat4 Perspective(float _fovY, float _aspect, float _near, float _far);
		Maths::Mat4 Orthographic(float _left, float _right, float _bottom, float _top);
	};

	class FreeCamera : public Camera
	{
	public:
		Maths::Vec3 eye, center, up;
		Maths::Vec3 forward;

		float camSpeed = 8.f;
		float camRotationSpeed = 0.1f;
		float zoomSpeed = 0.1f;

		TONIC_ENGINE_API FreeCamera(unsigned int _width, unsigned int _height);

		void TONIC_ENGINE_API SetView();
		void TONIC_ENGINE_API Update();

		void TONIC_ENGINE_API LookAt(float _x, float _y, float _z);
		void TONIC_ENGINE_API LookAt(const Maths::Vec3& _target);

		void TONIC_ENGINE_API Move(const Maths::Vec3& _velocity);
		void TONIC_ENGINE_API Turn(float _angle, Maths::Vec3 _axis);

		void TONIC_ENGINE_API ProcessInput(float _deltaTime, const CameraInput& _inputs);
		void TONIC_ENGINE_API Zoom(float _yoffset);
		void TONIC_ENGINE_API Teleport(Maths::Vec3 _position);
		void TONIC_ENGINE_API Teleport(Maths::Vec3 _position, Maths::Vec3 _target);
	};
}