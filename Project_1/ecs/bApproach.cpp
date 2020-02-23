#include "bApproach.h"
#include "cTransform.h"
#include "cVelocity.h"

#include <assert.h>
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
	assert(mAgent);
	assert(mTarget);

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
		agentVelocity->vx = 0;
		agentVelocity->vy = 0;

		if (!isShoot)
		{
			bulletTransform->position = agentTransform->position;
			isShoot = true;
			start = std::clock();
			currentTime = start;

			glm::vec3 desiredVelocity = glm::normalize(targetTransform->position - agentTransform->position);
			float magnitude = glm::length(targetTransform->position - agentTransform->position);
			desiredVelocity *= MAXVELOCITY;

			glm::vec3 steer;
			steer.x = desiredVelocity.x - bulletVelocity->vx;
			steer.y = desiredVelocity.y - bulletVelocity->vy;

			bulletVelocity->vx += steer.x * dt;
			bulletVelocity->vy += steer.y * dt;

			if (magnitude > MAXVELOCITY)
			{
				glm::vec3 normalized = { bulletVelocity->vx, bulletVelocity->vy, 0 };
				double mag = glm::length(normalized);
				normalized /= mag;

				bulletVelocity->vx = normalized.x * MAXVELOCITY;
				bulletVelocity->vy = normalized.y * MAXVELOCITY;
			}
		}
		else
		{
			if ((currentTime - start) / (float)CLOCKS_PER_SEC > 4.0f)
			{
				bulletTransform->position = agentTransform->position;
				bulletVelocity->vx = 0;
				bulletVelocity->vy = 0;
				isShoot = false;
			}
			else
			{
				currentTime = std::clock();
			}
		}
	}
	else
	{
		steer.x = desiredVelocity.x - agentVelocity->vx;
		steer.y = desiredVelocity.y - agentVelocity->vy;

		agentVelocity->vx += steer.x * dt;
		agentVelocity->vy += steer.y * dt;

		if (!isShoot)
		{
			bulletTransform->position = agentTransform->position;
			bulletVelocity->vx = agentVelocity->vx;
			bulletVelocity->vy = agentVelocity->vy;
		}
	}

	if (agentVelocity->vx > MAXVELOCITY)
		agentVelocity->vx = MAXVELOCITY;

	if (agentVelocity->vy > MAXVELOCITY)
		agentVelocity->vy = MAXVELOCITY;
}
