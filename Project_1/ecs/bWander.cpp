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

	//std::cout << "Desired Position before: " << mXPosition << " " << mYPosition << std::endl;

	if (mIsWandering == false && mIsIdling == false)
	{
		agentProperties->setDiffuseColour(glm::vec3(0.0f, 1.0f, 1.0f));
		mXPosition = rand() % (450 - (-450) + 1) + (-450);
		mYPosition = rand() % (450 - (-450) + 1) + (-450);
		
		mIsWandering = true;
	}

	//std::cout << "Desired Position after: " << mXPosition << " " << mYPosition << std::endl;

	glm::vec3 desiredPosition = glm::vec3(mXPosition, mYPosition, 0);

	glm::vec3 desiredVelocity = glm::normalize(desiredPosition - agentTransform->position);

	float magnitude = glm::length(desiredPosition - agentTransform->position);

	if (magnitude < 0.5)
	{			
		agentVelocity->vx = 0.0f;
		agentVelocity->vy = 0.0f;

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
	steer.x = desiredVelocity.x - agentVelocity->vx;
	steer.y = desiredVelocity.y - agentVelocity->vy;

	agentVelocity->vx += steer.x * dt;
	agentVelocity->vy += steer.y * dt;

	if (magnitude > MAXVELOCITY)
	{
		glm::vec3 normalized = { agentVelocity->vx, agentVelocity->vy, 0 };

		double mag = glm::length(normalized);

		normalized /= mag;

		agentVelocity->vx = normalized.x * MAXVELOCITY;
		agentVelocity->vy = normalized.y * MAXVELOCITY;
	}
}
