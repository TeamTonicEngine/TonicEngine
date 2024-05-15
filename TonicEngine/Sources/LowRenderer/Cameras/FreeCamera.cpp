#include "pch.hpp"

#include "LowRenderer/Cameras/FreeCamera.hpp"

using namespace Maths;
LowRenderer::Cameras::FreeCamera::FreeCamera(unsigned int _width, unsigned int _height) : Camera(_width, _height)
{
	bUsed = true;

	//Transform part
	eye = Vec3{ 0.f,0.f,-3.f };
	center = Vec3(0.f);
	up = Vec3::UP;
	forward = Vec3::FORWARD;
	// forward normalized inside SetView()

	camSpeed = 8.f;
	camRotationSpeed = 1.f;
	zoomSpeed = 0.1f;

	SetView();
	SetProjection();
	ComputeViewProjection();
}

// Remember to compute ViewProjection after this (or after projection)
void LowRenderer::Cameras::FreeCamera::SetView() { Camera::SetView(eye, forward, up); }

void LowRenderer::Cameras::FreeCamera::Move(const Vec3& _velocity)
{
	eye = eye + _velocity;
	center = center + _velocity;
}

void LowRenderer::Cameras::FreeCamera::Turn(float _angle, Vec3 _axis)
{
	Quat rotator = Quaternions::FromAngleAxis(_angle, _axis);
	forward = rotator.RotateVector(forward);
	up = rotator.RotateVector(up);
	//Insure the normalization
	forward = (forward).GetNormalized();
	up = (up).GetNormalized();

	if (bPerspectiveMode)
	{
		center = eye + forward;
		return;
	}
	else
		eye = center - forward * orthoScale;
}

void LowRenderer::Cameras::FreeCamera::LookAt(float _x, float _y, float _z) { LookAt({ _x,_y,_z }); }

void LowRenderer::Cameras::FreeCamera::LookAt(const Vec3& _target)
{
	center = _target;
	forward = (center - eye).GetNormalized();
	bViewChanged = true;
}

void LowRenderer::Cameras::FreeCamera::ProcessInput(float _deltaTime, const CameraInput& _input)
{
	float velocity = _deltaTime * camSpeed;

	if (_input.zoom)
	{
		Zoom(_input.zoom);
		bProjChanged = true;
	}
	if (_input.NoInputs())
		return;

	bViewChanged = true;
	if (_input.deltaX)
		Turn(_input.deltaX * _deltaTime * camRotationSpeed, Vec3::UP);
	if (_input.deltaY)
		Turn(_input.deltaY * _deltaTime * camRotationSpeed, Vectors::CrossProduct(up, forward).GetNormalized());

	// MOVEMENTS
	// ---------
	if (_input.bMoveForward)
		Move(forward * velocity);
	if (_input.bMoveBackward)
		Move(-forward * velocity);

	if (_input.bMoveUp)
		Move(up * velocity);
	if (_input.bMoveDown)
		Move(-up * velocity);

	if (_input.bMoveLeft)
	{
		Vec3 left = Vectors::CrossProduct(up, forward).GetNormalized();
		Move(left * velocity);
	}
	if (_input.bMoveRight)
	{
		Vec3 right = Vectors::CrossProduct(forward, up).GetNormalized();
		Move(right * velocity);
	}
}

void LowRenderer::Cameras::FreeCamera::Zoom(float _yoffset)
{
	if (bPerspectiveMode)
	{
		using namespace Constants;
		fovY -= _yoffset * Constants::DEG2RAD;
		if (fovY < (/*1.0f * */DEG2RAD)) //1 degree, so lets skip that computation
			fovY = /*1.0f * */DEG2RAD;
		if (fovY > 179.0f * DEG2RAD)
			fovY = 179.0f * DEG2RAD;
	}
	else
	{
		orthoScale -= zoomSpeed * _yoffset;
	}
	bProjChanged = true;
}

void LowRenderer::Cameras::FreeCamera::Teleport(Vec3 _position)
{
	eye = _position;
	center = eye + forward;
	SetView();
}

void LowRenderer::Cameras::FreeCamera::Teleport(Vec3 _position, Vec3 _target)
{
	eye = _position;
	center = _target;
	SetView();
}

void LowRenderer::Cameras::FreeCamera::Update()
{
	if (bViewChanged)
		SetView();

	if (bProjChanged)
		SetProjection();

	if (bViewChanged || bProjChanged)
		ComputeViewProjection();

	bViewChanged = false;
	bProjChanged = false;
}
bool LowRenderer::Cameras::CameraInput::NoInputs() const
{
	return !(deltaX != 0.f || deltaY != 0.f || bMoveForward || bMoveBackward || bMoveUp || bMoveDown || bMoveLeft || bMoveRight);
}