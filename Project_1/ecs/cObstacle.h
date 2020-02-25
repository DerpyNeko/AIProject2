#ifndef _HG_Obstacle_
#define _HG_Obstacle_

#include "Component.h"

class Obstacle : public Component
{
public:
	Obstacle(void) : radius(0.0f) { }
	virtual ~Obstacle(void) { }

	float radius;
};

#endif
