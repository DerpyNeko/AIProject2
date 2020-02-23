#include "sCollisionSystem.h"

#include <math.h>
#include "cTransform.h"
#include "cVelocity.h"
#include "cProperties.h"
#include <iostream>

struct CollisionPair
{
	CollisionPair(Entity* entityA, Entity* entityB) : entityA(entityA), entityB(entityB) { }

	Entity* entityA;
	Entity* entityB;

};

float const PI = 3.1415926;

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

	unsigned int numEntities = entities.size();
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
				if (!(propertyA->type == Type::PLAYER && propertyB->type == Type::BULLET))
					if (!(propertyA->type == Type::ENEMY && propertyB->type == Type::ENEMY))
						if (!(propertyA->type == Type::BULLET && propertyB->type == Type::BULLET))
							if (!(propertyA->type == Type::EBULLET && propertyB->type == Type::EBULLET))
								if (!(propertyA->type == Type::BULLET && propertyB->type == Type::EBULLET))
									if (!(propertyA->type == Type::EBULLET && propertyB->type == Type::BULLET))
										if (!(propertyA->type == Type::ENEMY && propertyB->type == Type::EBULLET))
											if (!(propertyA->type == Type::EBULLET && propertyB->type == Type::ENEMY))
												std::cout << entityA->name << " has collided with " << entityB->name << std::endl;

				if (propertyA->type == Type::PLAYER && propertyB->type == Type::ENEMY)
				{
					playerTransform->position = glm::vec3(0.0f, 0.0f, 0.0f);

					float circleRadius = 550.0f;
					float angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (PI * 2)));
					float x = sin(angle) * circleRadius;
					float y = cos(angle) * circleRadius;
					transformB->position = glm::vec3(x, y, 0.0f);
				}

				if (propertyA->type == Type::PLAYER && propertyB->type == Type::EBULLET)
				{
					playerTransform->position = glm::vec3(0.0f, 0.0f, 0.0f);

					transformB->position = glm::vec3(1000, 0, 0.0f);
					velocityB->vx = 0.0f;
					velocityB->vy = 0.0f;
				}

				if (propertyA->type == Type::ENEMY && propertyB->type == Type::BULLET)
				{
					float circleRadius = 550.0f;
					float angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (PI * 2)));
					float x = sin(angle) * circleRadius;
					float y = cos(angle) * circleRadius;

					transformA->position = glm::vec3(x, y, 0.0f);
					transformB->position = glm::vec3(-2500.0f, 300.0f, 0.0f);
					velocityB->vx = 0.0f;
					velocityB->vy = 0.0f;
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
