#ifndef _HG_PursueBehaviour_
#define _HG_PursueBehaviour_

#include "Behaviour.h"
#include "Entity.h"

#include <glm/vec3.hpp>

class PursueBehaviour : public Behaviour
{
public:
	PursueBehaviour(Entity* agent, Entity* target);

	virtual ~PursueBehaviour(void);
	virtual void Update(float dt);
	virtual std::string GetName(void);

private:
	Entity* mAgent;
	Entity* mTarget;

	bool BulletDetection(glm::vec3& closestBulletAvoidanceVector);
	void ClosestPtPointSegment(glm::vec3 c, glm::vec3 a, glm::vec3 b, float& t, glm::vec3& d);
};

#endif
