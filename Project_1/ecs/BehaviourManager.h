#ifndef _HG_Behaviour_Manager_
#define _HG_Behaviour_Manager_

//#include <map>
#include "Entity.h"
#include "Behaviour.h"

class BehaviourManager {
public:
	BehaviourManager(void);
	~BehaviourManager(void);

	void SetBehaviour(Entity* agent, Behaviour* behaviour);
	void RemoveAgent(Entity* agent, Behaviour* behaviour);

	void Update(float dt);

private:
	//std::map<Entity*, Behaviour*> mBehaviourMap;
	std::vector<std::pair<Entity*, Behaviour*>> mBehaviourVector;
};

//typedef std::map<Entity*, Behaviour*>::iterator behaviour_iterator;
typedef std::vector<std::pair<Entity*, Behaviour*>>::iterator behaviour_iterator;
#endif
