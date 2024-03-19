#pragma once
#include <TonicEngine/LowRenderer/Camera.hpp>

struct Input
{
	//Only for scene vizualizer Camera
	CameraInput cameraInput;
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