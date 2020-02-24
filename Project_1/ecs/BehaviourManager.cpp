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
	//behaviour_iterator itAgent = mBehaviourMap.find(agent);

	// If the agent is in the map, and there is already a behaviour then delete 
	// the behaviour
	//if (itAgent != mBehaviourMap.end() && itAgent->second != 0) 
	//{
		//delete itAgent->second;
	//}

	//mBehaviourMap[agent] = behaviour;
	mBehaviourVector.push_back(std::make_pair(agent, behaviour));
}

void BehaviourManager::RemoveAgent(Entity* agent, std::string behaviour) 
{
	for (behaviour_iterator it = mBehaviourVector.begin(); it != mBehaviourVector.end(); it++)
	{
		if (it->first == agent && it->second->GetName() == behaviour)
		{
			delete it->second;
			mBehaviourVector.erase(it);
			std::cout << "Deleting something" << std::endl;
			return;
		}
		else
		{
			std::cout << "Path Follow Behaviour not found, press 8 to instantiate a new one";
		}
	}

	// if the agent is in the map, delete the behaviour, and remove the agent.
	//if (std::find(mBehaviourVector.begin(), mBehaviourVector.end(), findPair) != mBehaviourVector.end()) 
	//{
	//	if (itAgent->second != 0) 
	//	{
	//		delete itAgent->second;
	//	}
	//	
	//	mBehaviourVector.erase(itAgent);
	//}
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