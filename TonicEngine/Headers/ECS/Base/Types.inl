#pragma once

#include <set>

#include "ECS/Base/Types.hpp"

namespace ECS
{
	// forward declarations
	class BaseSystem;
	struct BaseComponent;

	// return Component runtime type id
	inline const ComponentTypeID GetRuntimeComponentTypeID()
	{
		static ComponentTypeID typeID = 0u;
		return typeID++;
	}

	// return System runtime type id
	inline const SystemTypeID GetRuntimeSystemTypeID()
	{
		static SystemTypeID typeID = 0u;
		return typeID++;
	}

	// Attach type id to Component class and return it
	template<typename T>
	TONIC_ENGINE_API inline const ComponentTypeID CompType() noexcept
	{
		static_assert(std::is_base_of<BaseComponent, T>::value && !std::is_same<BaseComponent, T>::value, "INVALID TEMPLATE TYPE");
		static const ComponentTypeID typeID = GetRuntimeComponentTypeID();
		return typeID;
	}
	// Attach type id to System class and return it
	template<typename T>
	TONIC_ENGINE_API inline const SystemTypeID SystemType() noexcept
	{
		static_assert(std::is_base_of<BaseSystem, T>::value && !std::is_same<BaseSystem, T>::value, "INVALID TEMPLATE TYPE");
		static const SystemTypeID typeID = GetRuntimeSystemTypeID();
		return typeID;
	}
}