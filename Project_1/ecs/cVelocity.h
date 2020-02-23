#ifndef _HG_cVelocity_
#define _HG_cVelocity_

#include "Component.h"

struct Velocity : public Component
{
	Velocity(void) : vx(0), vy(0), ax(0), ay(0) { }
	Velocity(float vx, float vy) : vx(vx), vy(vy), ax(0), ay(0) { }
	Velocity(float vx, float vy, float ax, float ay) : vx(vx), vy(vy), ax(ax), ay(ay) { }

	virtual ~Velocity(void) { }

	float vx, vy;		// bulletVelocity in m/s
	float ax, ay;		// acceleration in m/s^2
};

#endif
