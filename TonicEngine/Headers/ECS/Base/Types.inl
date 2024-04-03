#pragma once

#include <set>

#include "Types.hpp"

namespace ECS
{
	// return Component runtime type id
	static const ComponentTypeID GetRuntimeComponentTypeID()
	{
		static ComponentTypeID typeID = 0u;
		return typeID++;
	}

	// return System runtime type id
	static const SystemTypeID GetRuntimeSystemTypeID()
	{
		static SystemTypeID typeID = 0u;
		return typeID++;
	}

	// forward declarations
	class BaseSystem;
	struct BaseComponent;

	//Attach type id to Component classs and return it
	template<typename T>
	static const ComponentTypeID CompType() noexcept
	{
		static_assert((std::is_base_of<BaseComponent, T>::value && !std::is_same<BaseComponent, T>::value), "INVALID TEMPLATE TYPE");
		static const ComponentTypeID typeID = GetRuntimeComponentTypeID();
		return typeID;
	}

	//Attach type id to System classs and return it
	template<typename T>
	static const  SystemTypeID SystemType() noexcept
	{
		static_assert((std::is_base_of<BaseSystem, T>::value && !std::is_same<BaseSystem, T>::value), "INVALID TEMPLATE TYPE");
		static const SystemTypeID typeID = GetRuntimeSystemTypeID();
		return typeID;
	}

	struct EntityData
	{
		EntityData() = default;
		~EntityData() = default;
		EntityData(EntityID _id, EntityID _parent) : id_(_id), parent_(_parent) {};

	private:
		friend class EntityRegistry;
		std::vector<EntityID> children_;
		EntityID id_;
		EntityID parent_;
	};
}