#include "EntityManager.h"

#include "Entity.h"

std::vector<Entity*> EntityManager::sEntities;

/**
 * Default constructor
 */
EntityManager::EntityManager(void)
{
	// Do nothing.
}

/**
 * Default Destructor
 */
EntityManager::~EntityManager(void)
{
	// Do nothing.
}

/**
 * CreateEntity
 * Creates an entity of hte provided type
 */
Entity* EntityManager::CreateEntity()
{
	Entity* newEntity = new Entity();
	EntityManager::sEntities.push_back(newEntity);
	return newEntity;
}

std::vector<Entity*> EntityManager::GetEntityList(void)
{
	return EntityManager::sEntities;
}

Entity* EntityManager::FindEntity(std::string name)
{
	for (Entity* e : EntityManager::sEntities)
	{
		if (e->name == name)
		{
			return e;
		}
	}

	return NULL;
}

