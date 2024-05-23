#pragma once
#include "DLL_API.hpp"

namespace Physics
{
	class PhysicsEngine
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	public:
		virtual const bool TONIC_ENGINE_API Init() = 0;

		virtual void TONIC_ENGINE_API Update(float _fixedDeltaTime) = 0;

		virtual void TONIC_ENGINE_API Destroy() = 0;

		virtual void* GetBodyInterface() = 0;
	};
}