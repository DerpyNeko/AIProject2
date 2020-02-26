#include "BehaviourManager.h"
#include <iostream>

BehaviourManager::BehaviourManager(void)
{
}

BehaviourManager::~BehaviourManager(void)
{
	// Perform cleanup
	for (behaviour_iterator itAgent = mBehaviourVector.begin(); itAgent != mBehaviourVector.end(); itAgent++)
	{
		if (itAgent->second != 0)
		{
			delete itAgent->second;
		}
	}

	mBehaviourVector.clear();
}

void BehaviourManager::SetBehaviour(Entity* agent, Behaviour* behaviour)
{
	// Check if the agent is already in the behaviour map
	for (behaviour_iterator it = mBehaviourVector.begin(); it != mBehaviourVector.end(); it++)
	{
		// Replaces the old behaviour with a new updated one with the same name (switching formations)
		if (it->first == agent && it->second->GetName() == behaviour->GetName())
		{
			delete it->second;
			mBehaviourVector.erase(it);
			std::cout << "Replacing with new behaviour" << std::endl;
			break;
		}
	}

	mBehaviourVector.push_back(std::make_pair(agent, behaviour));
}

void BehaviourManager::RemoveBehaviour(Entity* agent, std::string behaviour)
{
	int count = 0;

	for (behaviour_iterator it = mBehaviourVector.begin(); it != mBehaviourVector.end(); it++)
	{
		if (it->first == agent && it->second->GetName() == behaviour)
		{
			delete it->second;
			std::cout << "Count: " << count << std::endl;
			mBehaviourVector.erase(mBehaviourVector.begin() + count);
			std::cout << "Deleting " << behaviour << std::endl;
			std::cout << "Size after deletion is " << mBehaviourVector.size() << std::endl;
			return;
		}
		count++;
	}
}

void BehaviourManager::Update(float dt)
{
	if (mBehaviourVector.size() > 1)
	{
		if ((mBehaviourVector[0].second->GetName() == "FormationBehaviour"
			|| mBehaviourVector[0].second->GetName() == "FlockBehaviour")
			&& mBehaviourVector[1].second->GetName() == "PathFollowBehaviour")
		{
			std::pair<Entity*, Behaviour*> p = mBehaviourVector[0];
			mBehaviourVector[0] = mBehaviourVector[1];
			mBehaviourVector[1] = p;
		}
	}
	
	std::cout << "Current Behaviours: " << std::endl;

	for (behaviour_iterator itAgent = mBehaviourVector.begin(); itAgent != mBehaviourVector.end(); itAgent++)
	{
		if (itAgent->second != NULL)
		{
			itAgent->second->Update(dt);
		}

		std::cout << itAgent->second->GetName() << std::endl;
	}

	std::cout << "\n" << std::endl;
}