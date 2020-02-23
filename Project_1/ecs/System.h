#ifndef System_h
#define System_h

#include <vector>
#include <string>

#include "Entity.h"

class System
{
public:
	virtual ~System(void) { }

	virtual void Process(const std::vector<Entity*> &entities, float dt) = 0;

	const std::string& GetName()
	{
		return this->mName;
	}

protected:
	System(const std::string& name) : mName(name) { }

	std::string mName;					// The name of the system

private:
	System(void) { }
};

#endif
