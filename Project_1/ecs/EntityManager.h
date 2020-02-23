#ifndef EntityManager_h
#define EntityManager_h

#include <vector>
#include "Entity.h"

class EntityManager
{
public:
	EntityManager(void);
	virtual ~EntityManager(void);

	static Entity* CreateEntity(void);
	static std::vector<Entity*> GetEntityList(void);
	static Entity* FindEntity(std::string name);

protected:
	static std::vector<Entity*> sEntities;				// Container of entities
};

#endif
