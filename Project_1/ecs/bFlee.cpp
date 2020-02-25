#include "bFlee.h"
#include "bSeek.h"

#include "../globalStuff.h"

FleeBehaviour::FleeBehaviour(Entity* agent, Entity* target) : mAgent(agent), mTarget(target)
{
}

FleeBehaviour::~FleeBehaviour(void)
{
}

void FleeBehaviour::Update(float dt)
{
	// Gets nessesary components from entites
	Transform* agentTransform = mAgent->GetComponent<Transform>();
	Transform* targetTransform = mTarget->GetComponent<Transform>();
	Velocity* agentVelocity = mAgent->GetComponent<Velocity>();
	Properties* agentProperties = mAgent->GetComponent<Properties>();

	if (agentTransform == 0 || targetTransform == 0 || agentVelocity == 0) return;

	glm::vec3 agentForward = glm::normalize(glm::toMat3(agentTransform->getQOrientation()) * glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 targetForward = glm::normalize(glm::toMat3(targetTransform->getQOrientation()) * glm::vec3(0.0f, 0.0f, 1.0f));

	if (glm::dot(agentForward, targetForward) < 0)
	{
		gBehaviourManager.SetBehaviour(mAgent, new SeekBehaviour(mAgent, mTarget));
		agentProperties->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
		return;
	}

	glm::vec3 desiredVelocity = glm::normalize(agentTransform->position - targetTransform->position);

	desiredVelocity *= MAXVELOCITY;

	glm::vec3 steer;
	steer = desiredVelocity - agentVelocity->velocity;

	glm::quat orientation = glm::quat(glm::lookAt(agentTransform->position, steer, UP));

	orientation = glm::normalize(orientation);

	agentTransform->orientation = orientation;

	agentVelocity->velocity += steer * dt;

	if (agentVelocity->velocity.x > MAXVELOCITY)
		agentVelocity->velocity.x = MAXVELOCITY;

	if (agentVelocity->velocity.y > MAXVELOCITY)
		agentVelocity->velocity.y = MAXVELOCITY;
}

std::string FleeBehaviour::GetName(void)
{
	return "FleeBehaviour";
}
