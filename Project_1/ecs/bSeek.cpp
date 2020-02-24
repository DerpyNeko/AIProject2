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
	steer = desiredVelocity - agentVelocity->velocity;

	glm::quat orientation = glm::quat(glm::lookAt(agentTransform->position, agentTransform->position + steer, UP));

	orientation = glm::normalize(orientation);

	agentTransform->orientation = orientation;
	agentVelocity->velocity = steer * dt;

	if (magnitude > MAXVELOCITY)
	{
		glm::vec3 normalized = agentVelocity->velocity;
		double mag = glm::length(normalized);
		normalized /= mag;

		agentVelocity->velocity = normalized * MAXVELOCITY;
	}
}

std::string SeekBehaviour::GetName()
{
	return "SeekBehaviour";
}
