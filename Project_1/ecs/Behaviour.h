#ifndef _HG_Behaviour_
#define _HG_Behaviour_

#include "Component.h"
#include <string>

class Behaviour : public Component
{
public:
	virtual ~Behaviour(void) { }

	virtual void Update(float dt) = 0;

	float const MAXVELOCITY = 2.0f;

	virtual std::string GetName() = 0;

protected:
	Behaviour(void) { }
	std::string mBehaviourName;
};

#endif
