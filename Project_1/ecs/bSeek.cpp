#include "bSeek.h"
#include "bFlee.h"
#include "../globalStuff.h"

#include <assert.h>


SeekBehaviour::SeekBehaviour(Entity* agent, Entity* target): mAgent(agent), mTarget(target)
{
}

SeekBehaviour::~SeekBehaviour(void)
{
}

void SeekBehaviour::Update(float dt)
{
	assert(mAgent);
	assert(mTarget);

	// Gets nessesary components from entites
	Transform* agentTransform = mAgent->GetComponent<Transform>();
	Transform* targetTransform = mTarget->GetComponent<Transform>();
	Velocity* agentVelocity = mAgent->GetComponent<Velocity>();
	Properties* agentProperties = mAgent->GetComponent<Properties>();

	if (agentTransform == 0 || targetTransform == 0 || agentVelocity == 0) return;

	glm::vec3 agentForward = glm::normalize(glm::toMat3(agentTransform->getQOrientation()) * glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 targetForward = glm::normalize(glm::toMat3(targetTransform->getQOrientation()) * glm::vec3(0.0f, 0.0f, 1.0f));

	//std::cout << "dot " << glm::dot(agentForward, targetForward) << std::endl;

	if (glm::dot(agentForward, targetForward) > 0)
	{
		gBehaviourManager.SetBehaviour(mAgent, new FleeBehaviour(mAgent, mTarget));
		agentProperties->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
		return;
	}

	glm::vec3 desiredVelocity = glm::normalize(targetTransform->position - agentTransform->position);

	float magnitude = glm::length(targetTransform->position - agentTransform->position);

	desiredVelocity *= MAXVELOCITY;
	
	glm::vec3 steer;
	steer.x = desiredVelocity.x - agentVelocity->vx;
	steer.y = desiredVelocity.y - agentVelocity->vy;

	// glm::mat4 lookAt ( glm::vec3 eye, glm::vec3 center, glm::vec3 up )

	//glm::quat orientation = glm::quat(glm::lookAt(agentTransform->position, steer, UP));

	glm::quat orientation = glm::quat(glm::lookAt(agentTransform->position, agentTransform->position + steer, UP));

	orientation = glm::normalize(orientation);

	agentTransform->orientation = orientation;

	agentVelocity->vx += steer.x * dt;
	agentVelocity->vy += steer.y * dt;

	//agentVelocity->vx = 0;
	//agentVelocity->vy = 0;

	if (magnitude > MAXVELOCITY)
	{
		glm::vec3 normalized = { agentVelocity->vx, agentVelocity->vy, 0 };

		double mag = glm::length(normalized);

		normalized /= mag;

		agentVelocity->vx = normalized.x * MAXVELOCITY;
		agentVelocity->vy = normalized.y * MAXVELOCITY;
	}
}

