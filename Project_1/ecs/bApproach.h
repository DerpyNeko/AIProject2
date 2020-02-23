#ifndef _HG_ApproachBehaviour_
#define _HG_ApproachBehaviour_

#include "Behaviour.h"
#include "Entity.h"
#include "EntityManager.h"
#include "cProperties.h"

class ApproachBehaviour : public Behaviour
{
public:
	ApproachBehaviour(Entity* agent, Entity* target, Entity* bullet);
	virtual ~ApproachBehaviour(void);

	virtual void Update(float dt);

private:
	Entity* mAgent;
	Entity* mTarget;
	Entity* mBullet;

	bool isShoot;

	float const SLOWINGRADIUS = 400.0f;
	float const MAINTAINRADUIS = 150.0f;
};

#endif
