#include "sObstacleSystem.h"

#include <limits>

#include "cTransform.h"
#include "cVelocity.h"
#include "cObstacle.h"

void ObstacleSystem::Process(const std::vector<Entity*> &entities, float dt)
{
	Entity* entity;
	Transform* agentTransform;
	Velocity* agentVelocity;

	Entity* obstacle;
	Transform* obstacleTransform;
	Obstacle* obstacleInfo;

	Entity* closestObstacle;
	Transform* closestObstacleTransform;
	Obstacle* closestObstacleInfo;

	float closestObstacleDistance;
	glm::vec3 closestObstacleAvoidanceVector;

	for (unsigned int entityIndex = 0; entityIndex < entities.size(); entityIndex++)
	{
		entity = entities[entityIndex];

		// Ensure the components exist on the entity
		agentTransform = entity->GetComponent<Transform>();
		agentVelocity = entity->GetComponent<Velocity>();

		if (agentTransform == 0 || agentVelocity == 0) continue;

		glm::vec3 velocity = glm::vec3(agentVelocity->vx, agentVelocity->vy, 0.0f);
		if (glm::length(velocity) == 0.0f) continue;

		closestObstacleDistance = FLT_MAX;
		closestObstacle = 0;

		for (unsigned int obstacleIndex = 0; obstacleIndex < entities.size(); obstacleIndex++)
		{
			// Ensure it isn't the same entity
			if (obstacleIndex == entityIndex) continue;

			obstacle = entities[obstacleIndex];

			// Ensure the components exist on the obstacle
			obstacleTransform = obstacle->GetComponent<Transform>();
			obstacleInfo = obstacle->GetComponent<Obstacle>();

			if (obstacleTransform == 0 || obstacleInfo == 0) continue;

			glm::vec3 near = agentTransform->position + velocity * 1.0f;
			glm::vec3 far = agentTransform->position + velocity * 4.0f;

			float point;
			glm::vec3 closestPoint;
			ClosestPtPointSegment(obstacleTransform->position, far, near, point, closestPoint);

			float distance = glm::length(closestPoint - obstacleTransform->position);
			if (distance > obstacleInfo->radius + 1.0f) continue;

			if (distance < closestObstacleDistance)
			{
				// Set the obstacle as the closest.
				closestObstacle = obstacle;
				closestObstacleTransform = obstacleTransform;
				closestObstacleInfo = obstacleInfo;
				if (glm::length(closestPoint - obstacleTransform->position) == 0.0f) {
					// This needs to be handled. The obstacle is directly ahead of the agent
					// Which means there will be no avoidance direction to go.
					assert(0 && "This needs to be handled. The obstacle is directly ahead of the agent. Which means there will be no avoidance direction to go");
				}

				glm::vec3 avoidanceVector = glm::normalize(closestPoint - obstacleTransform->position);
				closestObstacleAvoidanceVector = avoidanceVector;
			}
		}

		// If there is a closest threat then perform this calculation
		if (closestObstacle != 0)
		{
			agentVelocity->vx = (velocity.x + closestObstacleAvoidanceVector.x) * 0.5f;
			agentVelocity->vy = (velocity.y + closestObstacleAvoidanceVector.y) * 0.5f;
		}
	}

}

// Christer Ericson - Real Time Collision Detection
// Section 5.1.2:
// Given segment ab and point c, computes closest point d on ab.
// Also returns t for the position of d, d(t) = a + t*(b - a)
void ObstacleSystem::ClosestPtPointSegment(glm::vec3 c, glm::vec3 a, glm::vec3 b, float &t, glm::vec3 &d)
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