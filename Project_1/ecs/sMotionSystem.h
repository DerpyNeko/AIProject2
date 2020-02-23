#ifndef _HG_sMotionSystem_
#define _HG_sMotionSystem_

#include "System.h"

class MotionSystem : public System
{
public:
	MotionSystem(void) : System("MotionSystem") { }
	virtual ~MotionSystem(void) { }

	virtual void Process(const std::vector<Entity*> &entities, float dt);
};

#endif
