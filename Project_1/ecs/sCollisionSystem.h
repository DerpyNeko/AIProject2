#ifndef _HG_cCollisionSystem_
#define _HG_cCollisionSystem_

#include "System.h"
#include <glm/glm.hpp>

class CollisionSystem : public System
{
public:
	CollisionSystem(void)
		: System("CollisionSystem") { }
	virtual ~CollisionSystem(void) { }

	virtual void Process(const std::vector<Entity*> &entities, float dt);

private:
	bool TestSphereSphereCollision(const glm::vec3 &pA, float rA, const glm::vec3 &pB, float rB);
};

#endif
