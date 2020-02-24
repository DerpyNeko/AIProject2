#ifndef _HG_cVelocity_
#define _HG_cVelocity_

#include "Component.h"

struct Velocity : public Component
{
	Velocity(void) : velocity(0, 0, 0), acceleration(0, 0, 0) { }
	Velocity(float vx, float vy) : velocity(vx, vy, 0), acceleration(0, 0, 0) { }
	Velocity(float vx, float vy, float ax, float ay) :velocity(vx, vy, 0), acceleration(ax, ay, 0) { }

	virtual ~Velocity(void) { }

	glm::vec3 velocity;			// bulletVelocity in m/s
	glm::vec3 acceleration;		// acceleration in m/s^2
};

#endif
