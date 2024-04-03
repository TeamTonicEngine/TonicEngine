#include "pch.hpp"

#include "LowRenderer/Camera.hpp"
#include "ECS/Components/Transform.hpp"

#include <cmath>
#include <iostream>

using namespace Maths;

LowRenderer::Cameras::Camera::Camera(unsigned int _width, unsigned int _height)
{
	zNear = 0.1f;
	zFar = 100.f;
	fovY = Constants::PI_2;

	// width and height are useful to compute projection matrix with the right aspect ratio
	width = _width;
	height = _height;
	aspect = (float)_width / _height;
	SetView(Vec3(0.f, 0.f, 0.f), Vec3::FORWARD, Vec3::UP);
	SetProjection();
	ComputeViewProjection();
}

void LowRenderer::Cameras::Camera::Update(Vec3 _position, Quat _rotation)
{
	//Only active cameras should be updated
	if (!bUsed)
		return;
	if (bViewChanged)
		SetView(_position, _rotation);
	if (bProjChanged)
		SetProjection();
	if (bViewChanged || bProjChanged)
		ComputeViewProjection();

	bViewChanged = false;
	bProjChanged = false;
}

// Remember to compute ViewProjection after this (or after view)
void LowRenderer::Cameras::Camera::SetView(Vec3 _position, Quat _rotation)
{
	using namespace Vectors;
	_rotation.ToEulerAngles();
	Vec3 zC = _rotation.RotateVector(Vec3{ 0.f,0.f,-1.f });
	Vec3 xC = _rotation.RotateVector(Vec3{ 0.f,1.f,1.f });
	Vec3 yC = CrossProduct(zC, xC);

	view = {
	xC.x, yC.x, zC.x, 0.f,
	xC.y, yC.y, zC.y, 0.f,
	xC.z, yC.z, zC.z, 0.f,
	-DotProduct(xC, _position), -DotProduct(yC, _position), -DotProduct(zC, _position), 1.f
	};
}

// Remember to compute ViewProjection after this (or after projection)
void LowRenderer::Cameras::Camera::SetView(Vec3 _position, Vec3 _forward, Vec3 _up)
{
	using namespace Vectors;
	Vec3 zC = -_forward;
	Vec3 xC = CrossProduct(_up, zC).GetNormalized();
	Vec3 yC = CrossProduct(zC, xC);

	view = {
	xC.x, yC.x, zC.x, 0.f,
	xC.y, yC.y, zC.y, 0.f,
	xC.z, yC.z, zC.z, 0.f,
	-DotProduct(xC, _position), -DotProduct(yC, _position), -DotProduct(zC, _position), 1.f
	};
}

// Remember to compute ViewProjection after this (or after view)
void LowRenderer::Cameras::Camera::SetProjection()
{
	if (bPerspectiveMode)
		projection = Perspective(fovY, aspect, zNear, zFar);
	else
		projection = Orthographic(-orthoScale, orthoScale, -orthoScale, orthoScale);
}

void LowRenderer::Cameras::Camera::ComputeViewProjection() { viewProjection = projection * view; }

/*
void LowRenderer::Cameras::Camera::ShowImGuiControls()
{
	// View
	if (ImGui::CollapsingHeader("Controls", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("WASD Keys to move along XZ Axis");
		ImGui::Text("Right click to capture mouse and");
		ImGui::Text("Move to turn around %s.", bPerspectiveMode ? "camera" : "object");
	}
	if (ImGui::CollapsingHeader("View", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::SliderFloat("Camera Speed", &camSpeed, 0.f, 10.f);
		ImGui::SliderFloat("Angle Sensibility", &camRotationSpeed, 0.01f, 1.0f);
		ImGui::SliderFloat3("eye", eye.elements, -1000.f, 1000.f);
		if (ImGui::IsItemEdited())
			bViewChanged = true;
		ImGui::SliderFloat3("target", center.elements, -1000.f, 1000.f);
		if (ImGui::IsItemEdited())
			bViewChanged = true;
		ImGui::SliderFloat3("up", up.elements, 0.f, 1.f);
		if (ImGui::IsItemEdited())
			bViewChanged = true;

		static Vec3 lookAt;
		ImGui::InputFloat3("Look At :", &lookAt[0]);
		if (ImGui::IsItemEdited())
			LookAt(lookAt);
	}
	// Projection
	if (ImGui::CollapsingHeader("Projection", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::ListBoxHeader("Options", ImVec2(100, 36)))
		{
			if (ImGui::Selectable("Perspective", bPerspectiveMode))
			{
				bPerspective Mode = true;
				bProjChanged = true;
			}
			if (ImGui::Selectable("Orthographic", !bPerspectiveMode))
			{
				bPerspective Mode = false;
				bProjChanged = true;
			}
			ImGui::ListBoxFooter();
		}
		ImGui::SliderAngle("FOV", &fovY, 1.f, 179.f);
		if (ImGui::IsItemEdited() && bPerspectiveMode)
			bProjChanged = true;
		ImGui::SliderFloat("Near", &zNear, 0.f, 10.f);
		if (ImGui::IsItemEdited() && bPerspectiveMode)
			bProjChanged = true;
		ImGui::SliderFloat("Far", &zFar, 1.f, 10000.f);
		if (ImGui::IsItemEdited() && bPerspectiveMode)
			bProjChanged = true;
	}
}
*/

Mat4 LowRenderer::Cameras::Camera::Frustum(float _left, float _right, float _bottom, float _top, float _near, float _far)
{
	return Matrices::Frustum(_left, _right, _bottom, _top, _near, _far);
}

Mat4 LowRenderer::Cameras::Camera::Perspective(float _fovY, float _aspect, float _near, float _far)
{
	return Matrices::Perspective(_fovY, _aspect, _near, _far);
}

Mat4 LowRenderer::Cameras::Camera::Orthographic(float _left, float _right, float _bottom, float _top)
{
	return Matrices::Ortho(_left, _right, _bottom, _top, zNear, zFar);
}

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