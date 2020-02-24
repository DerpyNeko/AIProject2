#include "bPathFollow.h"

#include <assert.h>

#include "cTransform.h"
#include "cVelocity.h"
#include <iostream>

#include "../globalStuff.h"

#include <math.h>

bool isStopped = false;
bool isReversed = false;
double const PI = 3.1415926;
bool isBorked = false;

PathFollowingBehaviour::PathFollowingBehaviour(Entity* entity, Path* path) 
	: mEntity(entity) , mPath(path) , mCurrentGoal(0), mPathRadius(15.0f)
{
	assert(mEntity, "PathFollowingBehaviour: The entity is null.");
	assert(mPath, "PathFollowingBehaviour: The path is null.");
}

PathFollowingBehaviour::~PathFollowingBehaviour(void)
{
}

void PathFollowingBehaviour::Update(float dt)
{
	if (!isStopped)
	{
		// Check if the entity is at the goal position
		Transform* agentTransform = mEntity->GetComponent<Transform>();
		assert(transform, "PathFollowingBehaviour: Entity does not contain a Transform component.");
		Velocity* agentVelocity = mEntity->GetComponent<Velocity>();
		assert(transform, "PathFollowingBehaviour: Entity does not contain a Velocity component.");

		PathNode pathNode = mPath->pathNodes[mCurrentGoal];
		Properties* nodeProperties = g_PathNodes[mCurrentGoal]->GetComponent<Properties>();
		nodeProperties->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));

		float dist = glm::distance(agentTransform->position, pathNode.position);

		if (dist < mPathRadius)
		{
			if (mCurrentGoal == mPath->pathNodes.size() - 1 || isReversed && mCurrentGoal == 0)
			{
				agentVelocity->velocity = glm::vec3(0.0f);
				return;
			}

			nodeProperties->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
			isBorked = false;

			if (isReversed && mCurrentGoal != 0)
			{
				mCurrentGoal--;
			}
			else if (!isReversed)
			{
				mCurrentGoal++;
			}
		}

		glm::vec3 desiredVelocity = glm::normalize(mPath->pathNodes[mCurrentGoal].position - agentTransform->position);

		float magnitude = glm::length(pathNode.position - agentTransform->position);

		desiredVelocity *= MAXVELOCITY;

		//glm::vec3 steer;
		//steer = desiredVelocity - agentVelocity->velocity;

		//glm::quat orientation = glm::quat(glm::lookAt(agentTransform->position, agentTransform->position + steer, UP));

		//std::cout << orientation.z << std::endl;
		//orientation = glm::normalize(orientation);


		//agentTransform->orientation = orientation;	
		//agentTransform->adjMeshOrientationEulerAngles(glm::vec3(0, 0, orientation.z), true);

		//agentVelocity->velocity = steer * dt;

		glm::vec3 steer;
		steer.x = desiredVelocity.x - agentVelocity->velocity.x;
		steer.y = desiredVelocity.y - agentVelocity->velocity.y;

		glm::quat orientation = glm::quat(glm::lookAt(agentTransform->position, agentTransform->position + steer, UP));

		orientation = glm::normalize(orientation);

		if (!isBorked)
		{
			std::cout << orientation.z << std::endl;
			//agentTransform->orientation = orientation;

			float angle = 0.0f;
			//agentTransform->orientation.z = orientation.z;
			if (desiredVelocity.y < agentVelocity->velocity.y)
				angle = atan(steer.x / steer.y);
			else
				angle = atan(steer.y / steer.x);

			agentTransform->adjMeshOrientationEulerAngles(glm::vec3(0, 0, angle), false);
			isBorked = true;
		}


		//agentTransform->adjMeshOrientationEulerAngles(glm::vec3(0, 0, orientation.z), true);
		//agentTransform->orientation = orientation;


		agentVelocity->velocity.x += steer.x * dt;
		agentVelocity->velocity.y += steer.y * dt;

		if (magnitude > MAXVELOCITY)
		{
			glm::vec3 normalized = agentVelocity->velocity;
			double mag = glm::length(normalized);
			normalized /= mag;

			agentVelocity->velocity = normalized * MAXVELOCITY;
		}
	}
	else
	{
		mEntity->GetComponent<Velocity>()->velocity = glm::vec3(0.0f);
	}
}