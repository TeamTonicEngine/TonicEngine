#pragma once
#include <DLL_API.hpp>

#include "Maths/Maths.hpp"

namespace Core::Renderer { class OpenGLWrapper; }

namespace LowRenderer::Cameras
{
	class TONIC_ENGINE_API Camera
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	public:
		// Required only for an active camera
		Maths::Mat4 view;
		Maths::Mat4 projection;
		Maths::Mat4 viewProjection;

		unsigned int width, height;
		float aspect;
		float fovY;
		float zNear, zFar;
		float orthoScale = 2.f;

		bool bPerspectiveMode = true;
		bool bViewChanged;
		bool bProjChanged;
		bool bUsed = false;

		friend class Core::Renderer::OpenGLWrapper;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/

		Camera(unsigned int _width, unsigned int _height);

		void Update(Maths::Vec3 _position, Maths::Quat _rotation);
		void SetView(Maths::Vec3 _position, Maths::Vec3 _forward, Maths::Vec3 _up);
		void SetView(Maths::Vec3 _position, Maths::Quat _rotation);
		void SetProjection();
		void ComputeViewProjection();

	protected:
		Maths::Mat4 Frustum(float _left, float _right, float _bottom, float _top, float _near, float _far);
		Maths::Mat4 Perspective(float _fovY, float _aspect, float _near, float _far);
		Maths::Mat4 Orthographic(float _left, float _right, float _bottom, float _top);
	};
}