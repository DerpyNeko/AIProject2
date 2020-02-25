#include "bApproach.h"
#include "cTransform.h"
#include "cVelocity.h"

#include <ctime>

float currentTime = 0.0f;
float start = 0.0f;

ApproachBehaviour::ApproachBehaviour(Entity* agent, Entity* target, Entity* bullet)
	: mAgent(agent), mTarget(target), mBullet(bullet)
{
	isShoot = false;
}

ApproachBehaviour::~ApproachBehaviour(void)
{
}

void ApproachBehaviour::Update(float dt)
{
	// Gets nessesary components from entites
	Transform* agentTransform = mAgent->GetComponent<Transform>();
	Transform* targetTransform = mTarget->GetComponent<Transform>();
	Transform* bulletTransform = mBullet->GetComponent<Transform>();
	Velocity* agentVelocity = mAgent->GetComponent<Velocity>();
	Velocity* bulletVelocity = mBullet->GetComponent<Velocity>();

	if (agentTransform == 0 || targetTransform == 0 || agentVelocity == 0) return;

	glm::vec3 desiredVelocity = glm::normalize(targetTransform->position - agentTransform->position);

	glm::vec3 distance = targetTransform->position - agentTransform->position;

	distance.x = fabs(distance.x);
	distance.y = fabs(distance.y);

	if (distance.x < SLOWINGRADIUS && distance.y < SLOWINGRADIUS)
	{
		desiredVelocity = desiredVelocity * MAXVELOCITY * (distance / SLOWINGRADIUS);
	}
	else
	{
		desiredVelocity *= MAXVELOCITY;
	}

	glm::vec3 steer;

	if (distance.x < MAINTAINRADUIS && distance.y < MAINTAINRADUIS)
	{
		agentVelocity->velocity = glm::vec3(0, 0, 0);

		if (!isShoot)
		{
			bulletTransform->position = agentTransform->position;
			isShoot = true;
			start = (float)(std::clock());
			currentTime = start;

			glm::vec3 desiredVelocity = glm::normalize(targetTransform->position - agentTransform->position);
			float magnitude = glm::length(targetTransform->position - agentTransform->position);
			desiredVelocity *= MAXVELOCITY;

			glm::vec3 steer;
			steer = desiredVelocity - bulletVelocity->velocity;
			bulletVelocity->velocity += steer * dt;

			if (magnitude > MAXVELOCITY)
			{
				glm::vec3 normalized = bulletVelocity->velocity;
				double mag = glm::length(normalized);
				normalized /= mag;

				bulletVelocity->velocity = normalized * MAXVELOCITY;
			}
		}
		else
		{
			if ((currentTime - start) / (float)CLOCKS_PER_SEC > 4.0f)
			{
				bulletTransform->position = agentTransform->position;
				bulletVelocity->velocity = glm::vec3(0, 0, 0);
				isShoot = false;
			}
			else
			{
				currentTime = (float)(std::clock());
			}
		}
	}
	else
	{
		steer = desiredVelocity - agentVelocity->velocity;
		agentVelocity->velocity += steer * dt;

		if (!isShoot)
		{
			bulletTransform->position = agentTransform->position;
			bulletVelocity->velocity = agentVelocity->velocity;
		}
	}

	if (agentVelocity->velocity.x > MAXVELOCITY)
		agentVelocity->velocity.x = MAXVELOCITY;

	if (agentVelocity->velocity.y > MAXVELOCITY)
		agentVelocity->velocity.y = MAXVELOCITY;
}

std::string ApproachBehaviour::GetName()
{
	return "ApproachBehaviour";
}
