#pragma once
#include "pch.hpp"

#include "Types.hpp"

namespace ECS
{
	struct BaseComponent
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
		TONIC_ENGINE_API BaseComponent()
			: entityID_() {};
		virtual TONIC_ENGINE_API ~BaseComponent() = default;

		inline const TONIC_ENGINE_API EntityID GetID() const { return entityID_; };

		friend class EntityManager;
	};
}