#ifndef _HG_WanderBehaviour_
#define _HG_WanderBehaviour_

#include "Behaviour.h"
#include "Entity.h"

class WanderBehaviour : public Behaviour
{
public:
	WanderBehaviour(Entity* agent);
	virtual ~WanderBehaviour(void);

	virtual void Update(float dt);

private:
	Entity* mAgent;
	bool mIsWandering;
	bool mIsIdling;
	float mXPosition;
	float mYPosition;
	float mStartIdleTime;
};

#endif
