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
	virtual std::string GetName(void);

private:
	Entity* mAgent;
	bool mIsWandering;
	bool mIsIdling;
	float mXPosition;
	float mYPosition;
	float mStartIdleTime;
};

#endif
