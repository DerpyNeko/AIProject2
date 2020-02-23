#ifndef _HG_ObstacleSystem_
#define _HG_ObstacleSystem_

#include "System.h"
#include "cTransform.h"

class ObstacleSystem : public System
{
public:
	ObstacleSystem(void) : System("ObstacleSystem") { }
	virtual ~ObstacleSystem(void) { }

	virtual void Process(const std::vector<Entity*> &entities, float dt);

private:
	void ClosestPtPointSegment(glm::vec3 c, glm::vec3 a, glm::vec3 b, float &t, glm::vec3 &d);
};

#endif
