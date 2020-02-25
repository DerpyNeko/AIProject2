#ifndef _HG_Behaviour_Manager_
#define _HG_Behaviour_Manager_

#include "Behaviour.h"

class BehaviourManager {
public:
	BehaviourManager(void);
	~BehaviourManager(void);

	void SetBehaviour(Entity* agent, Behaviour* behaviour);
	void RemoveBehaviour(Entity* agent, std::string behaviour);

	void Update(float dt);

private:
	std::vector<std::pair<Entity*, Behaviour*>> mBehaviourVector;
};

typedef std::vector<std::pair<Entity*, Behaviour*>>::iterator behaviour_iterator;

#endif
