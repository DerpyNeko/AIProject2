#ifndef _HG_FleeBehaviour_
#define _HG_FleeBehaviour_

#include "Behaviour.h"
#include "Entity.h"

class FleeBehaviour : public Behaviour
{
public:
	FleeBehaviour(Entity* agent, Entity* target);
	virtual ~FleeBehaviour(void);

	virtual void Update(float dt);

private:
	Entity* mAgent;
	Entity* mTarget;
};

#endif
