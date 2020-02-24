#ifndef _HG_SeekBehaviour_
#define _HG_SeekBehaviour_

#include "Behaviour.h"
#include "Entity.h"

class SeekBehaviour : public Behaviour
{
public:
	SeekBehaviour(Entity* agent, Entity* target);
	virtual ~SeekBehaviour(void);

	virtual void Update(float dt);

	virtual std::string GetName();

private:
	Entity* mAgent;
	Entity* mTarget;
};

#endif
