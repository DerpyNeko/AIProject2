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
	for (behaviour_iterator it = mBehaviourVector.begin(); it != mBehaviourVector.end(); it++)
	{
		if (it->first == agent && it->second->GetName() == behaviour)
		{
			delete it->second;
			mBehaviourVector.erase(it);
			std::cout << "Deleting behaviour" << std::endl;
			return;
		}
		else
		{
			std::cout << "Path Follow Behaviour not found, press 8 to instantiate a new one";
		}
	}
}

void BehaviourManager::Update(float dt)
{
	std::cout << "Current behaviours: " << std::endl;

	for (behaviour_iterator itAgent = mBehaviourVector.begin(); itAgent != mBehaviourVector.end(); itAgent++)
	{
		if (itAgent->second != 0)
		{
			itAgent->second->Update(dt);
		}

		std::cout << itAgent->first->name << " " << itAgent->second->GetName() << std::endl;
	}

	std::cout << "\n" << std::endl;
}