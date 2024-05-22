#pragma once
#include "DLL_API.hpp"
#include "ECS/Base/BaseComponent.hpp"

namespace ECS
{
	namespace Components
	{
		struct TONIC_ENGINE_API AudioMixerComponent : public BaseComponent
		{
			AudioMixerComponent() = default;
		protected:
		};
	}
}