#include "sCollisionSystem.h"

#include <math.h>

#include "cTransform.h"
#include "cVelocity.h"
#include "cProperties.h"

#include "../globalStuff.h"

struct CollisionPair
{
	CollisionPair(Entity* entityA, Entity* entityB) : entityA(entityA), entityB(entityB) { }

	Entity* entityA;
	Entity* entityB;
};

float const PI = (float)(3.141592);

void CollisionSystem::Process(const std::vector<Entity*>& entities, float dt)
{
	Entity* entityA;
	Entity* entityB;

	Properties* propertyA;
	Properties* propertyB;

	Transform* transformA;
	Transform* transformB;

	Velocity* velocityA;
	Velocity* velocityB;

	std::vector<CollisionPair*> collisions;

	Transform* playerTransform = g_player->GetComponent<Transform>();

	unsigned int numEntities = (unsigned int)(entities.size());
	for (unsigned int idxA = 1; idxA < numEntities; idxA++)
	{
		entityA = entities[idxA];
		transformA = entityA->GetComponent<Transform>();
		propertyA = entityA->GetComponent<Properties>();
		velocityA = entityA->GetComponent<Velocity>();

		for (unsigned int idxB = idxA + 1; idxB < numEntities; idxB++)
		{
			entityB = entities[idxB];
			transformB = entityB->GetComponent<Transform>();
			propertyB = entityB->GetComponent<Properties>();
			velocityB = entityB->GetComponent<Velocity>();

			if (TestSphereSphereCollision(transformA->position, transformA->sphereRadius, transformB->position, transformB->sphereRadius))
			{
				if (!(propertyA->type == eType::PLAYER && propertyB->type == eType::BULLET))
					if (!(propertyA->type == eType::ENEMY && propertyB->type == eType::ENEMY))
						if (!(propertyA->type == eType::BULLET && propertyB->type == eType::BULLET))
							if (!(propertyA->type == eType::EBULLET && propertyB->type == eType::EBULLET))
								if (!(propertyA->type == eType::BULLET && propertyB->type == eType::EBULLET))
									if (!(propertyA->type == eType::EBULLET && propertyB->type == eType::BULLET))
										if (!(propertyA->type == eType::ENEMY && propertyB->type == eType::EBULLET))
											if (!(propertyA->type == eType::EBULLET && propertyB->type == eType::ENEMY))
												std::cout << entityA->name << " has collided with " << entityB->name << std::endl;

				if (propertyA->type == eType::PLAYER && propertyB->type == eType::ENEMY)
				{
					playerTransform->position = glm::vec3(0.0f, 0.0f, 0.0f);

					float circleRadius = 550.0f;
					float angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (PI * 2)));
					float x = sin(angle) * circleRadius;
					float y = cos(angle) * circleRadius;
					transformB->position = glm::vec3(x, y, 0.0f);
				}

				if (propertyA->type == eType::PLAYER && propertyB->type == eType::EBULLET)
				{
					playerTransform->position = glm::vec3(0.0f, 0.0f, 0.0f);

					transformB->position = glm::vec3(1000, 0, 0.0f);
					velocityB->velocity = glm::vec3(0, 0, 0);
				}

				if (propertyA->type == eType::ENEMY && propertyB->type == eType::BULLET)
				{
					float circleRadius = 550.0f;
					float angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (PI * 2)));
					float x = sin(angle) * circleRadius;
					float y = cos(angle) * circleRadius;

					transformA->position = glm::vec3(x, y, 0.0f);
					transformB->position = glm::vec3(-2500.0f, 300.0f, 0.0f);
					velocityB->velocity = glm::vec3(0, 0, 0);
				}
			}
		}
	}
}

bool CollisionSystem::TestSphereSphereCollision(const glm::vec3& pA, float rA, const glm::vec3& pB, float rB)
{
	float xDiff = pA.x - pB.x;
	float yDiff = pA.y - pB.y;

	float distance = sqrt(xDiff * xDiff + yDiff * yDiff);
	float totalRadius = rA + rB;

	return distance < totalRadius;
}
