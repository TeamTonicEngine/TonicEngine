#pragma once

#include "ECS/Base/BaseSystem.hpp"

#include "Resources/Font.hpp"

namespace ECS::Systems
{
	class TextRendererSystem : public ECS::BaseSystem
	{
		std::map<u32/*priority*/, EntityID> sortedEntities_;

	public:
		TONIC_ENGINE_API TextRendererSystem();
		TONIC_ENGINE_API ~TextRendererSystem();
		void TONIC_ENGINE_API AddEntity(const EntityID _entity) override;
		void TONIC_ENGINE_API RemoveEntity(const EntityID _entity) override;
		const bool TONIC_ENGINE_API Init() override;
		void TONIC_ENGINE_API Update() override;
		void TONIC_ENGINE_API Render() override;
		void TONIC_ENGINE_API RenderEditorScene() override;
		void ChangePriority(ECS::EntityID _entity, u32 _newPriority);
	};
}