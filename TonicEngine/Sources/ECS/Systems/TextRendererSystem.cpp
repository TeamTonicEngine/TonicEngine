#include "pch.hpp"

#include "ECS/Systems/TextRendererSystem.hpp"
#include "ECS/Components/TextRendererComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"

#include "Resources/Shader.hpp"
#include "Resources/Font.hpp"

ECS::Systems::TextRendererSystem::TextRendererSystem()
{
	AddComponentSignature<Components::TransformComponent>();
	AddComponentSignature<Components::TextRendererComponent>();
}

ECS::Systems::TextRendererSystem::~TextRendererSystem()
{
	entities_.clear();
	sortedEntities_.clear();
}

void ECS::Systems::TextRendererSystem::AddEntity(const EntityID _entity)
{
	//avoid doubles
	if (entities_.find(_entity) != entities_.end())
		return;

	entities_.insert(_entity);
	Components::TextRendererComponent txtRdr = ENGINE.ENT_MNGR->GetComponent<Components::TextRendererComponent>(_entity);

	while (sortedEntities_.find(txtRdr.priority) != sortedEntities_.end())
		txtRdr.priority++;

	sortedEntities_.emplace(txtRdr.priority, _entity);
}

void ECS::Systems::TextRendererSystem::RemoveEntity(const EntityID _entity)
{
	auto it = entities_.find(_entity);
	if (it != entities_.end())
		entities_.erase(it);

	for (auto it = sortedEntities_.begin(); it != sortedEntities_.end();)
	{
		EntityID entity = it->second;
		if (entity == _entity)
		{
			sortedEntities_.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

const bool ECS::Systems::TextRendererSystem::Init()
{
	for (auto entity : entities_)
	{
		Components::TextRendererComponent txtRdr = ENGINE.ENT_MNGR->GetComponent<Components::TextRendererComponent>(entity);
		sortedEntities_.emplace(txtRdr.priority, entity);
	}
	bool success = true;

	if (success)
		DEBUG_SUCCESS("Initialized Text Renderer System")
	else
		DEBUG_ERROR("Failed to initialize Text Renderer System");
	return success;
}

void ECS::Systems::TextRendererSystem::Update()
{
	//Shortcut to EntityManager
	ECS::EntityManager* p_EM = ENGINE.ENT_MNGR;
	for (auto entity : entities_)
	{
		auto* transformComp = &p_EM->GetComponent<Components::TransformComponent>(entity);

		auto* textComp = &p_EM->GetComponent<Components::TextRendererComponent>(entity);

		if (transformComp->HasChanged())
		{
			//Update stored model matrix
			textComp->model_ = transformComp->GetModel();
		}
	}

	//Check if priorities have changed
	for (auto it = sortedEntities_.begin(); it != sortedEntities_.end();)
	{
		EntityID entity = it->second;
		auto* textComp = &p_EM->GetComponent<Components::TextRendererComponent>(entity);
		if (!textComp)
			continue;
		if (it->first != textComp->priority)
		{
			while (sortedEntities_.find(textComp->priority) != sortedEntities_.end())
				textComp->priority++;
			sortedEntities_.emplace(textComp->priority, entity);
			sortedEntities_.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

void ECS::Systems::TextRendererSystem::Render()
{
	if (sortedEntities_.size() == 0u)
		return;

	//Shortcut to EntityManager
	ECS::EntityManager* p_EM = ENGINE.ENT_MNGR;
	std::map<u32/*priority*/, EntityID> sortedSeparatedEntities = sortedEntities_;
	Resources::Font::s_p_textShader->Use();

	//Shortcut to Renderer
	auto p_RDR = ENGINE.RDR;

	//3D part
	for (auto it = sortedSeparatedEntities.cbegin(); it != sortedSeparatedEntities.cend();)
	{
		EntityID entity = it->second;
		auto* textComp = &p_EM->GetComponent<Components::TextRendererComponent>(entity);
		if (!textComp || !textComp->b3D)
		{
			++it;
			continue;
		}
		if (!textComp->bEnabled)
		{
			sortedSeparatedEntities.erase(it++);
			continue;
		}
		auto* transformComp = &p_EM->GetComponent<Components::TransformComponent>(entity);

		textComp->p_font->UseWithoutSafety();

		p_RDR->TransformText(textComp->model_);

		p_RDR->RenderText(textComp->text, transformComp->position.x, transformComp->position.y, transformComp->scale, textComp->color);
		sortedSeparatedEntities.erase(it++);
	}
	if (sortedSeparatedEntities.size() == 0u)
		return;

	//2D part (LateRender ?)
	p_RDR->DrawOnTop();
	for (auto it = sortedSeparatedEntities.begin(); it != sortedSeparatedEntities.end(); ++it)
	{
		EntityID entity = it->second;
		auto* textComp = &p_EM->GetComponent<Components::TextRendererComponent>(entity);
		if (!textComp->bEnabled)
			continue;
		
		auto* transformComp = &p_EM->GetComponent<Components::TransformComponent>(entity);

		textComp->p_font->UseWithoutSafety();
		p_RDR->TransformText(textComp->model_);

		p_RDR->RenderText(textComp->text, transformComp->position.x, transformComp->position.y, transformComp->scale, textComp->color);
	}
	p_RDR->StopUseFonts();
}

void ECS::Systems::TextRendererSystem::RenderEditorScene() { Render(); }	//Could put any Editor linked

void ECS::Systems::TextRendererSystem::ChangePriority(ECS::EntityID _entity, u32 _newPriority)
{
	Components::TextRendererComponent txtRdr = ENGINE.ENT_MNGR->GetComponent<Components::TextRendererComponent>(_entity);

	while (sortedEntities_.find(txtRdr.priority) != sortedEntities_.end())
		txtRdr.priority++;

	auto it = sortedEntities_.find(txtRdr.priority);
	if (it->second == _entity)
		sortedEntities_.erase(it);

	sortedEntities_.emplace(_newPriority, _entity);
}