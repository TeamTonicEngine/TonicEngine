#pragma once

#include <set>

namespace ECS
{
	// constants
	constexpr u64 MAX_ENTITY_COUNT = 4096;
	constexpr u64 MAX_COMP_COUNT = 32;

	// custom types
	using EntityID = u64;
	using SystemTypeID = u64;
	using ComponentTypeID = u64;
	using EntitySignature = std::set<ComponentTypeID>;

	constexpr EntityID INVALID_ENTITY_ID = -1;
	constexpr EntityID ROOT_ENTITY_ID = 0;

	/* - Output : Component runtime type id */
	const ComponentTypeID TONIC_ENGINE_API GetRuntimeComponentTypeID();
	/* - Output : System runtime type id */
	const SystemTypeID TONIC_ENGINE_API GetRuntimeSystemTypeID();

	/* Attach a type id to a Component class and returns it */
	template<typename T>
	const  ComponentTypeID TONIC_ENGINE_API CompType() noexcept;
	/* Attach a type id to a System class and returns it */
	template<typename T>
	const  SystemTypeID TONIC_ENGINE_API SystemType() noexcept;


}

#include "Types.inl"