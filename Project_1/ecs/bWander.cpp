#include "bWander.h"
#include "../globalStuff.h"

#include <assert.h>

WanderBehaviour::WanderBehaviour(Entity* agent) : mAgent(agent)
{
	mIsWandering = false;
	mIsIdling = false;
	mXPosition = 0.0f;
	mYPosition = 0.0f;
	mStartIdleTime = 0.0f;
}

WanderBehaviour::~WanderBehaviour(void)
{
}

void WanderBehaviour::Update(float dt)
{
	assert(mAgent);

	Transform* agentTransform = mAgent->GetComponent<Transform>();
	Velocity* agentVelocity = mAgent->GetComponent<Velocity>();
	Properties* agentProperties = mAgent->GetComponent<Properties>();

	if (agentTransform == 0 || agentVelocity == 0) return;

	double currentTime = glfwGetTime();

	if (mIsWandering == false && mIsIdling == false)
	{
		agentProperties->setDiffuseColour(glm::vec3(0.0f, 1.0f, 1.0f));
		mXPosition = rand() % (450 - (-450) + 1) + (-450);
		mYPosition = rand() % (450 - (-450) + 1) + (-450);
		
		mIsWandering = true;
	}

	glm::vec3 desiredPosition = glm::vec3(mXPosition, mYPosition, 0);

	glm::vec3 desiredVelocity = glm::normalize(desiredPosition - agentTransform->position);

	float magnitude = glm::length(desiredPosition - agentTransform->position);

	if (magnitude < 0.5)
	{
		agentVelocity->velocity = glm::vec3(0, 0, 0);

		// Sets starting idling time if wander position is reached
		if (mIsIdling == false)
		{
			agentProperties->setDiffuseColour(glm::vec3(1, 0.5,0));

			mStartIdleTime = currentTime;
			mIsIdling = true;	
			mIsWandering = false;
		}

		// After 3 seconds, start to wander again
		if (currentTime - mStartIdleTime > 3.0f)
		{
			mIsIdling = false;
			mIsWandering = false;
		}
	}
	
	desiredVelocity *= MAXVELOCITY;

	glm::vec3 steer;
	steer = desiredVelocity - agentVelocity->velocity;
	agentVelocity->velocity += steer * dt;

	if (magnitude > MAXVELOCITY)
	{
		glm::vec3 normalized = agentVelocity->velocity;
		double mag = glm::length(normalized);
		normalized /= mag;

		agentVelocity->velocity = normalized * MAXVELOCITY;
	}
}

std::string WanderBehaviour::GetName()
{
	return "WanderBehaviour";
}
