#pragma once
#include "pch.hpp"

#include "Types.hpp"

namespace ECS
{
	struct TONIC_ENGINE_API BaseComponent
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		EntityID entityID_;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		BaseComponent()
			: entityID_() {};
		virtual ~BaseComponent() = default;

		inline const EntityID GetID() const { return entityID_; };

		friend class EntityManager;
	};
}