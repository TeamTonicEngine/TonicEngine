#pragma once

#include "Types.hpp"

#include <iostream>

namespace ECS
{
	class BaseSystem
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		EntitySignature signature_;
		std::set<EntityID> entities_;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		TONIC_ENGINE_API BaseSystem() = default;
		TONIC_ENGINE_API virtual ~BaseSystem() = default;

		TONIC_ENGINE_API void AddEntity(const EntityID _entity);
		TONIC_ENGINE_API void RemoveEntity(const EntityID _entity);
		TONIC_ENGINE_API const EntitySignature GetSignature() const;

		template<typename T>
		void AddComponentSignature() { signature_.insert(CompType<T>()); };

		TONIC_ENGINE_API virtual const bool Init();
		TONIC_ENGINE_API virtual void Update();
		TONIC_ENGINE_API virtual void LateUpdate();
		TONIC_ENGINE_API virtual void Render();
		TONIC_ENGINE_API virtual void RenderEditorScene();
		TONIC_ENGINE_API virtual void Destroy();

		friend class EntityManager;
	};
}