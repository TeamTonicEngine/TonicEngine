#include "pch.hpp"

#include "Core/Log.hpp"
#include "ECS/Base/EntityManager.hpp"
#include "ECS/Base/EntityData.inl"

#include "ECS/Components.hpp"

ECS::EntityManager::EntityManager() : entityCount_(0)
{
	for (EntityID entity = 0; entity < MAX_ENTITY_COUNT; entity++)
		availableEntities_.push(entity);
	EntityID root = AddNewEntity();
	if (root != ROOT_ENTITY_ID)
		DEBUG_ERROR("Uh-oh, Entities were not correctly cleaned, root is not at the beginning of the scene");
	entities_.at(root)->parent_ = INVALID_ENTITY_ID;
}

bool ECS::EntityManager::Init()
{
	for (auto& system : registeredSystems_)
	{
		if (!system.second->Init())
			return false;
	}
	return true;
}

ECS::EntityManager::~EntityManager()
{
	Destroy();
}

void ECS::EntityManager::Update()
{
	for (auto& system : registeredSystems_)
		system.second->Update();
}
void ECS::EntityManager::LateUpdate()
{
	for (auto& system : registeredSystems_)
		system.second->LateUpdate();
}
void ECS::EntityManager::Render()
{
	for (auto& system : registeredSystems_)
		system.second->Render();
}
void ECS::EntityManager::RenderEditorScene()
{
	for (auto& system : registeredSystems_)
		system.second->RenderEditorScene();
}
void ECS::EntityManager::Destroy()
{
	for (auto& system : registeredSystems_)
		system.second->Destroy();
}
const ECS::EntityID ECS::EntityManager::AddNewEntity()
{
	const EntityID entityID = availableEntities_.front();
	AddEntityData(entityID);
	availableEntities_.pop();
	entityCount_++;
	return entityID;
}

const void ECS::EntityManager::AddEntityWithId(const EntityID _id)
{
	std::queue<EntityID> tempQueue;

	while (!availableEntities_.empty()) {
		EntityID current = availableEntities_.front();
		availableEntities_.pop();

		if (current != _id) {
			tempQueue.push(current);
		}
	}

	availableEntities_ = tempQueue;

	AddEntityData(_id);

	entityCount_++;
}

void ECS::EntityManager::DestroyEntity(const EntityID _entity)
{
	if (_entity == ROOT_ENTITY_ID)
	{
		DEBUG_WARNING("Root entity can't be destroyed.");
		return;
	}
	if (_entity + 1 > MAX_ENTITY_COUNT)
	{
		DEBUG_WARNING("EntityID out of range ! Entity nb%u can't be destroyed as it doesn't exist.", (size_t)_entity);
		return;
	}
	if (entities_[_entity]->HasChildren())
		for (auto child : entities_[_entity]->GetChildren())
			ParentEntity(child, entities_[_entity]->parent_);

	entities_[entities_[_entity]->GetParent()]->RemoveChild(entities_[_entity]->GetID());

	//Remove in referencement
	for (auto& components : componentsArrays_)
		components.second->Erase(_entity);
	for (auto& system : registeredSystems_)
		system.second->RemoveEntity(_entity);

	entities_.erase(_entity);

	//Update the used/avalaible entities
	entityCount_--;
	availableEntities_.push(_entity);
}

void ECS::EntityManager::DestroyAllEntitiesFromEntity(const EntityID _entity)
{
	if (entities_[_entity]->HasChildren())
		for (auto child : entities_[_entity]->GetChildren())
		{
			DestroyAllEntitiesFromEntity(child);

			DestroyEntity(child);
		}
}

void ECS::EntityManager::ResetAvailableEntities()
{
	std::queue<EntityID> tempQueue;
	for (EntityID entity = 1; entity < MAX_ENTITY_COUNT; entity++) // important entity = 1 to not add the root
		tempQueue.push(entity);
	availableEntities_ = tempQueue;
}

void ECS::EntityManager::AddEntityData(const EntityID _entity)
{
	if (entities_.find(_entity) != entities_.end())
	{
		DEBUG_WARNING("Entity not found! Can't add signature !");
		return;
	}
	entities_[_entity] = std::move(std::make_shared <EntityData>(_entity, this));
}

ECS::EntitySignature* ECS::EntityManager::GetEntitySignature(const EntityID& _entity) const
{
	if (entities_.find(_entity) == entities_.end())
	{
		DEBUG_WARNING("Entity not found! Can't get signature !");
		return nullptr;
	}
	return &entities_.at(_entity)->components_;
}

std::shared_ptr<ECS::EntityData> ECS::EntityManager::GetEntityData(const EntityID& _entity)
{
	if (entities_.find(_entity) == entities_.end())
	{
		DEBUG_WARNING("Entity not found! Can't get data !");
		return nullptr;
	}
	return entities_.at(_entity);
}

void ECS::EntityManager::AddChild(EntityID _parent, EntityID _child)
{
	//Root Case
	if (_child == ROOT_ENTITY_ID)
	{
		DEBUG_WARNING("Cannot reparent the root !")
			return;
	}
	EntityID ancestor = _parent;
	while (ancestor != ROOT_ENTITY_ID)
	{
		if (ancestor == _child)
		{
			DEBUG_WARNING("GrandFather paradox, Fry, the entity can't be its own father/child !")
				return;
		}
		ancestor = GetParent(ancestor);
	}
	//Add Child to new parent's list
	std::vector<EntityID>& children = GetEntityData(_parent)->children_;
	auto it = std::find_if(children.cbegin(), children.cend(), [&](EntityID _existingChild) {return _existingChild == _child; });
	if (it != children.cend())
	{
		DEBUG_WARNING("The new child is already child of the entity !")
			return;
	}
	children.push_back(_child);

	//Remove Child from old parent's list
	std::vector<EntityID>& oldChildren = GetEntityData(GetEntityParent(_child))->children_;
	auto oldIt = std::find_if(oldChildren.cbegin(), oldChildren.cend(), [&](EntityID _existingChild) {return _existingChild == _child; });
	oldChildren.erase(oldIt);

	GetEntityData(_child)->parent_ = _parent;
}

void ECS::EntityManager::RemoveChild(EntityID _parent, EntityID _child)
{
	std::vector<EntityID>& children = GetEntityData(_parent)->children_;
	auto it = std::find_if(children.cbegin(), children.cend(), [&](EntityID _existingChild) {return _existingChild == _child; });
	if (it == children.cend())
	{
		DEBUG_WARNING("The child is not in the child list of the entity !")
			return;
	}
	children.erase(it);
}

void ECS::EntityManager::ParentEntity(const EntityID _entity, const EntityID _parent)
{
	AddChild(_parent, _entity);
}

void ECS::EntityManager::ParentEntityToRoot(const EntityID _entity)
{
	//Root Case
	if (_entity == ROOT_ENTITY_ID)
	{
		DEBUG_WARNING("Cannot reparent the root !")
			return;
	}
	GetEntityData(ROOT_ENTITY_ID)->children_.push_back(_entity);
	if (entities_.find(_entity) != entities_.end())
		entities_.at(_entity)->parent_ = ROOT_ENTITY_ID;
}

ECS::EntityID ECS::EntityManager::GetEntityParent(const EntityID& _entity) const
{
	if (entities_.find(_entity) == entities_.end())
	{
		DEBUG_WARNING("Entity not found! Can't get parent !");
		return INVALID_ENTITY_ID;
	}
	return entities_.at(_entity)->parent_;
}
std::vector<ECS::EntityID> ECS::EntityManager::GetEntityChildren(const EntityID& _entity) const
{
	if (entities_.find(_entity) == entities_.end())
	{
		DEBUG_WARNING("Entity not found! Can't get children !");
		return std::vector<ECS::EntityID>(); //empty vector
	}
	return entities_.at(_entity)->children_;
}
void ECS::EntityManager::UpdateEntityTargetSystems(const EntityID _entity)
{
	for (auto& system : registeredSystems_)
		AddEntityToSystem(_entity, system.second.get());
}
ECS::EntityID ECS::EntityManager::GetEntityChild(const EntityID& _entity, size_t _childIdx) const
{
	std::vector<ECS::EntityID> children = GetEntityChildren(_entity);
	if (children.size() < _childIdx)
	{
		DEBUG_WARNING("Child index out of range! Can't get child !");
		return INVALID_ENTITY_ID;
	}
	return children.at(_childIdx);
}
void ECS::EntityManager::AddEntityToSystem(const EntityID _entity, BaseSystem* _p_system)
{
	if (BelongToSystem(_entity, _p_system->signature_))
		_p_system->AddEntity(_entity);
	else
		_p_system->RemoveEntity(_entity);
}

const bool ECS::EntityManager::BelongToSystem(const EntityID& _entity, const EntitySignature& _systemSignature) const
{
	for (ComponentTypeID compType : _systemSignature)
		if (GetEntitySignature(_entity)->count(compType) == 0)
			return false;

	return true;
}