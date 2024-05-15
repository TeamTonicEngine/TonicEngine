#include "pch.hpp"

#include "LowRenderer/Cameras/Camera.hpp"

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

	this->view = {
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
	{
		fovY = std::clamp(fovY, Maths::Constants::PI_4, Maths::Constants::PI_3 *2.f);
		projection = Perspective(fovY, aspect, zNear, zFar);
	}
	else
	{
		orthoScale = std::clamp(orthoScale, 0.0001f, 10000000.f);
		projection = Orthographic(-orthoScale, orthoScale, -orthoScale, orthoScale);
	}
}

void LowRenderer::Cameras::Camera::ComputeViewProjection() { viewProjection = projection * view; }

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