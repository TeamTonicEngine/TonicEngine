#pragma once
#include "pch.hpp"

#include <set>

namespace ECS
{
	// constants
	constexpr size_t MAX_ENTITY_COUNT = 5000;
	constexpr size_t MAX_COMP_COUNT = 32;

	// custom types
	using EntityID = size_t;
	using SystemTypeID = size_t;
	using ComponentTypeID = size_t;
	using EntitySignature = std::set<ComponentTypeID>;

	constexpr EntityID INVALID_ENTITY_ID = -1;

	/* - Output : Component runtime type id */
	static const ComponentTypeID GetRuntimeComponentTypeID();
	/* - Output : System runtime type id */
	static const SystemTypeID GetRuntimeSystemTypeID();

	/* Attach a type id to a Component class and returns it */
	template<typename T>
	static const ComponentTypeID CompType() noexcept;
	/* Attach a type id to a System class and returns it */
	template<typename T>
	static const  SystemTypeID SystemType() noexcept;

}

#include "Types.inl"