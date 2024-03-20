#include "pch.hpp"
#include "TonicEngine/LowRenderer/Camera.hpp"

#include <cmath>
#include <iostream>

using namespace Maths;
Camera::Camera(unsigned int _width, unsigned int _height)
{
	zNear = 0.1f;
	zFar = 100.f;
	fovY = Constants::PI_2;
	//Transform part
	eye = Vec3{ 0.f,0.f,-3.f };
	center = Vec3{ 0.f,0.f,0.f };
	up = Vec3{ 0.f,1.f,0.f };
	// forward normalized inside SetView()

	// width and height are useful to compute projection matrix with the right aspect ratio
	width = _width;
	height = _height;
	aspect = (float)_width / _height;
	SetView();
	SetProjection();
	ComputeViewProjection();
}
void Camera::Update()
{
	//Only active cameras should be updated
	if (!bUsed)
		return;
	if (bViewChanged)
		SetView();
	if (bProjChanged)
		SetProjection();
	if (bViewChanged || bProjChanged)
		ComputeViewProjection();

	bViewChanged = false;
	bProjChanged = false;
}

void Camera::Move(const Vec3& _velocity)
{
	eye = eye + _velocity;
	center = center + _velocity;
}

void Camera::Turn(float _angle, Vec3 _axis)
{
	Quat rotator = Quaternions::FromAngleAxis(_angle, _axis);
	forward = rotator.RotateVector(forward);
	up = rotator.RotateVector(up);
	//Insure the normalization
	forward=(forward).GetNormalized();
	up = (up).GetNormalized();

	if (bPerspectiveMode)
	{
		center = eye + forward;
		return;
	}
	else
		eye = center - forward * orthoScale;
}


// Remember to compute ViewProjection after this (or after projection)
void Camera::SetView()
{
	view = Matrices::LookAt(forward, up);
	forward = (center - eye).GetNormalized();
	using namespace Vectors;
	Vec3 zC = -forward;
	Vec3 xC = CrossProduct(up, zC).GetNormalized();
	Vec3 yC = CrossProduct(zC, xC);

	view = {
	xC.x, yC.x, zC.x, 0.f,
	xC.y, yC.y, zC.y, 0.f,
	xC.z, yC.z, zC.z, 0.f,
	-DotProduct(xC, eye), -DotProduct(yC, eye), -DotProduct(zC, eye), 1.f
		};
}

// Remember to compute ViewProjection after this (or after view)
void Camera::SetProjection()
{
	if (bPerspectiveMode)
		projection = Perspective(fovY, aspect, zNear, zFar);
	else
		projection = Orthographic(-orthoScale, orthoScale, -orthoScale, orthoScale);
}

void Camera::ComputeViewProjection() {
	viewProjection = projection * view;
}

void Camera::LookAt(float _x, float _y, float _z) {
	LookAt({ _x,_y,_z });
}

void Camera::LookAt(const Vec3& _target)
{
	center = _target;
	forward = (center - eye).GetNormalized();
	bViewChanged = true;
}
/*
void Camera::ShowImGuiControls()
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

Mat4 Camera::Frustum(float _left, float _right, float _bottom, float _top, float _near, float _far)
{
	return Matrices::Frustum(_left, _right, _bottom, _top, _near, _far);
}

Mat4 Camera::Perspective(float _fovY, float _aspect, float _near, float _far)
{
	return Matrices::Perspective(_fovY, _aspect, _near, _far);
}

Mat4 Camera::Orthographic(float _left, float _right, float _bottom, float _top)
{
	return Matrices::Ortho(_left, _right, _bottom, _top, zNear, zFar);
}


FreeCamera::FreeCamera(unsigned int _width, unsigned int _height) :Camera(_width, _height)
{
	camSpeed = 8.f;
	camRotationSpeed = 1.f;
	zoomSpeed = 0.1f;
}

void FreeCamera::ProcessInput(float _deltaTime, const CameraInput& _input)
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
	
	if (_input.deltaY)
		Turn(_input.deltaY * _deltaTime * camRotationSpeed, Vectors::CrossProduct(up, forward).GetNormalized());
	if (_input.deltaX)
		Turn(_input.deltaX * _deltaTime * camRotationSpeed, Vec3::UP);
	// MOVEMENTS
	// ---------
	if (_input.bMoveForward)
		Move(forward * velocity);
	if (_input.bMoveBackward)
		Move(-forward * velocity);

	if (_input.bMoveUp)
		Move(up * velocity);
	if (_input.bMoveDown)
		Move(-up* velocity);

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


void FreeCamera::Zoom(float _yoffset)
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
void FreeCamera::Teleport(Vec3 _position)
{
	eye = _position;
	center = eye + forward;
	SetView();
}
void FreeCamera::Teleport(Vec3 _position, Vec3 _target)
{
	eye = _position;
	center = _target;
	SetView();
}

bool CameraInput::NoInputs() const
{
	{
		if (deltaX != 0.f || deltaY != 0.f || bMoveForward || bMoveBackward || bMoveUp || bMoveDown || bMoveLeft || bMoveRight)
			return false;
		return true;
	}
}
