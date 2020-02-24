#include "sMotionSystem.h"

#include "cTransform.h"
#include "cVelocity.h"

/**
 * Process
 * Calculates all movement of the entities which contain a
 * Position component and Velocity component.
 * @param entities - the list of entities to process
 */
void MotionSystem::Process(const std::vector<Entity*> &entities, float dt)
{
	Entity* entity;

	for (unsigned int i = 0; i < entities.size(); i++)
	{
		entity = entities[i];

		Transform* transform = entity->GetComponent<Transform>();
		Velocity* velocity = entity->GetComponent<Velocity>();

		if (transform == 0 || velocity == 0) continue;

		velocity->velocity += velocity->acceleration* dt;
		transform->position += velocity->velocity;

#ifdef LOG_SYSTEMS
		printf("    acc (%.2f, %.2f)\n    vel (%.2f, %.2f)\n    pos (%.2f, %.2f)\n"
			, velocity->ax, velocity->ay
			, velocity->vx, velocity->vy
			, transform->position.x, transform->position.y);
#endif
	}
}