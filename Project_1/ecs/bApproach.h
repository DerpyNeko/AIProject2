#ifndef _HG_ApproachBehaviour_
#define _HG_ApproachBehaviour_

#include "Behaviour.h"

class ApproachBehaviour : public Behaviour
{
public:
	ApproachBehaviour(Entity* agent, Entity* target, Entity* bullet);

	virtual ~ApproachBehaviour(void);
	virtual void Update(float dt);
	virtual std::string GetName(void);

private:
	Entity* mAgent;
	Entity* mTarget;
	Entity* mBullet;

	bool isShoot;

	float const SLOWINGRADIUS = 400.0f;
	float const MAINTAINRADUIS = 150.0f;
};

#endif
