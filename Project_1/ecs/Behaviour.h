#ifndef _HG_Behaviour_
#define _HG_Behaviour_

#include "Component.h"
#include <string>
#include <iostream>

#include "cTransform.h"
#include "cVelocity.h"
#include "cProperties.h"

#include "Entity.h"
#include "EntityManager.h"

class Behaviour : public Component
{
public:
	virtual ~Behaviour(void) {}
	virtual void Update(float dt) = 0;
	virtual std::string GetName(void) = 0;

protected:
	Behaviour(void) {}
	float const MAXVELOCITY = 2.0f;
};

#endif
