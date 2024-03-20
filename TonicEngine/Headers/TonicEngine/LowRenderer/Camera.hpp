#pragma once
#include "TonicEngine/Core/Maths.hpp"
#include <TonicEngine/DLL_API.hpp>

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
class TONIC_ENGINE_API Camera
{
public:
	//Required only for active camera
	Maths::Mat4 view;
	Maths::Mat4 projection;
	Maths::Mat4 viewProjection;

	Maths::Vec3 eye, center, up;
	Maths::Vec3 forward;

	unsigned int width, height;
	float aspect;
	float fovY;
	float zNear, zFar;
	float orthoScale = 2.f;

	bool bViewChanged;
	bool bPerspectiveMode = true;
	bool bProjChanged;
	bool bUsed = false;

	Camera(unsigned int _width, unsigned int _height);

	void Update();
	void SetView();
	void SetProjection();
	void ComputeViewProjection();
	void LookAt(float _x, float _y, float _z);
	void LookAt(const Maths::Vec3& _target);

	//void ShowImGuiControls();

protected:
	Maths::Mat4 Frustum(float _left, float _right, float _bottom, float _top, float _near, float _far);
	Maths::Mat4 Perspective(float _fovY, float _aspect, float _near, float _far);
	Maths::Mat4 Orthographic(float _left, float _right, float _bottom, float _top);

	void Move(const Maths::Vec3& _velocity);
	void Turn(float _angle, Maths::Vec3 _axis);
};
class TONIC_ENGINE_API FreeCamera : public Camera
{
public:
	FreeCamera(unsigned int _width, unsigned int _height);

	float camSpeed = 8.f;
	float camRotationSpeed = 0.1f;
	float zoomSpeed = 0.1f;

	void ProcessInput(float _deltaTime, const CameraInput& _inputs);
	void Zoom(float _yoffset);
	void Teleport(Maths::Vec3 _position);
	void Teleport(Maths::Vec3 _position, Maths::Vec3 _target);
};
