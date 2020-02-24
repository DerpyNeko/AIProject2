#include "bPursue.h"
#include "../globalStuff.h"

#include <assert.h>

PursueBehaviour::PursueBehaviour(Entity* agent, Entity* target) : mAgent(agent), mTarget(target)
{
}

PursueBehaviour::~PursueBehaviour(void)
{
}

void PursueBehaviour::Update(float dt)
{
	assert(mAgent);
	assert(mTarget);

	// Gets nessesary components from entites
	Transform* agentTransform = mAgent->GetComponent<Transform>();
	Transform* targetTransform = mTarget->GetComponent<Transform>();
	Velocity* agentVelocity = mAgent->GetComponent<Velocity>();
	Velocity* targetVelocity = mTarget->GetComponent<Velocity>();
	Properties* agentProperties = mAgent->GetComponent<Properties>();

	if (agentTransform == 0 || targetTransform == 0 || agentVelocity == 0) return;

	float magnitude = glm::length(targetTransform->position - agentTransform->position);

	int T = (int)magnitude / (int)MAXVELOCITY;

	glm::vec3 futurePosition;
	futurePosition = targetTransform->position + targetVelocity->velocity * glm::vec3(T, T, 0);

	glm::vec3 desiredVelocity = glm::normalize(futurePosition - agentTransform->position);

	desiredVelocity *= MAXVELOCITY;

	glm::vec3 avoidanceVector = glm::vec3(0.0f, 0.0f, 0.0f);
	bool bIsThreatDetected = BulletDetection(avoidanceVector);

	glm::vec3 steer;

	if (bIsThreatDetected)
	{
		agentProperties->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
		agentVelocity->velocity = agentVelocity->velocity + avoidanceVector;
	}
	else
	{
		agentProperties->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));

		steer = desiredVelocity - agentVelocity->velocity;
		agentVelocity->velocity += steer * dt;
	}

	if (magnitude > MAXVELOCITY)
	{
		glm::vec3 normalized = agentVelocity->velocity;
		double mag = glm::length(normalized);
		normalized /= mag;

		agentVelocity->velocity = normalized * MAXVELOCITY;
	}
}

bool PursueBehaviour::BulletDetection(glm::vec3& closestBulletAvoidanceVector)
{
	Transform* agentTransform = mAgent->GetComponent<Transform>();
	Velocity* agentVelocity = mAgent->GetComponent<Velocity>();

	Entity* closestBullet = 0;
	Transform* closestBulletTransform;

	float closestBulletDistance = FLT_MAX;

	if (agentTransform == 0 || agentVelocity == 0) return false;

	glm::vec3 velocity = agentVelocity->velocity;
	if (glm::length(velocity) == 0.0f) return false;

	for (Entity* e : g_bullets)
	{
		Transform* bulletTransform = e->GetComponent<Transform>();

		if (bulletTransform == 0) continue;

		glm::vec3 close = agentTransform->position + velocity * 2.0f;
		glm::vec3 away = agentTransform->position + velocity * 50.0f;

		float point;
		glm::vec3 closestPoint;

		ClosestPtPointSegment(bulletTransform->position, away, close, point, closestPoint);

		float distance = glm::length(closestPoint - bulletTransform->position);
		if (distance > bulletTransform->sphereRadius + 20.0f) continue;

		if (distance < closestBulletDistance)
		{
			// Set the obstacle as the closest.
			closestBullet = e;
			closestBulletTransform = bulletTransform;
			if (glm::length(closestPoint - bulletTransform->position) == 0.0f) {
				// This needs to be handled. The obstacle is directly ahead of the agent
				// Which means there will be no avoidance direction to go.
				assert(0 && "This needs to be handled. The obstacle is directly ahead of the agent. Which means there will be no avoidance direction to go");
			}

			glm::vec3 avoidanceVector = glm::normalize(closestPoint - bulletTransform->position);
			closestBulletAvoidanceVector = avoidanceVector;
		}
	}

	if (closestBullet != 0)
	{
		return true;
	}

	return false;
}

// Christer Ericson - Real Time Collision Detection
// Section 5.1.2:
// Given segment ab and point c, computes closest point d on ab.
// Also returns t for the position of d, d(t) = a + t*(b - a)
void PursueBehaviour::ClosestPtPointSegment(glm::vec3 c, glm::vec3 a, glm::vec3 b, float& t, glm::vec3& d)
{
	glm::vec3 ab = b - a;
	// Project c onto ab, computing parameterized position d(t) = a + t*(b – a)
	t = glm::dot(c - a, ab) / glm::dot(ab, ab);
	// If outside segment, clamp t (and therefore d) to the closest endpoint
	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;
	// Compute projected position from the clamped t
	d = a + t * ab;
}
