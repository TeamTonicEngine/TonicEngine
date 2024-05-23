#pragma once

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
		bool bEnabled_ = true;

	public:
		virtual const bool GetEnabled() { return bEnabled_; }
		virtual void SetEnabled(bool _value) { bEnabled_ = _value; }
		__declspec(property(get = GetEnabled, put = SetEnabled))
			bool bEnabled;
		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
		BaseComponent()
			: entityID_() {};
		virtual ~BaseComponent() = default;

		inline const EntityID GetID() const { return entityID_; };

		friend class EntityManager;
	};
}