#include "sCollisionSystem.h"

#include <math.h>

#include "cTransform.h"
#include "cVelocity.h"
#include "cProperties.h"

#include "../globalStuff.h"

int intersect_moving_sphere_sphere(
	const glm::vec3& c0, float r0, const glm::vec3& v0,
	const glm::vec3& c1, float r1, const glm::vec3& v1, float& t);

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

				if ((propertyA->type == eType::PLAYER || propertyA->type == eType::OTHER) && propertyB->type == eType::OTHER)
				{
					glm::vec3 cA = transformA->previousPosition;
					glm::vec3 vA = transformA->position - transformA->previousPosition;
					float rA = transformA->scale.x;
					glm::vec3 cB = transformB->previousPosition;
					glm::vec3 vB = transformB->position - transformB->previousPosition;
					float rB = transformB->scale.x;

					float t = 0.f;
					int result = intersect_moving_sphere_sphere(cA, rA, vA, cB, rB, vB, t);

					if (result == 0 || result == -1) return;

					transformA->position = transformA->previousPosition + vA * t;
					transformB->position = transformB->previousPosition + vB * t;

					float ma = 30.0f;
					float mb = 30.0f;
					float mt = ma + mb;
					glm::vec3 va = velocityA->velocity;
					glm::vec3 vb = velocityB->velocity;
					float c = 0.2f;

					velocityA->velocity = (c * mb * (vb - va) + ma * va + mb * vb) / mt;
					velocityB->velocity = (c * ma * (va - vb) + ma * va + mb * vb) / mt;
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

int intersect_moving_sphere_sphere(
	const glm::vec3& c0, float r0, const glm::vec3& v0,
	const glm::vec3& c1, float r1, const glm::vec3& v1, float& t)
{
	glm::vec3 s = c1 - c0;  // sphere center separation
	glm::vec3 v = v1 - v0;  // relative motion of sphere 1 w.r.t. stationary s0
	float r = r1 + r0; // sum of sphere radii
	float c = glm::dot(s, s) - r * r;
	if (c < 0.f)
	{
		// spheres initially overlapping, exit early
		t = 0.f;
		return -1;
	}
	float a = glm::dot(v, v);
	if (a < FLT_EPSILON) return 0; // spheres not moving relative to eachother
	float b = glm::dot(v, s);
	if (b >= 0.f) return 0; // spheres not moving towards eachother
	float d = b * b - a * c;
	if (d < 0.f) return 0;  // no real root, so spheres do not intersect

	t = (-b - glm::sqrt(d)) / a;
	return t < 1.f ? 1 : 0;
}
