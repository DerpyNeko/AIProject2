#ifndef _HG_FLOCK_
#define _HG_FLOCK_

#include "Entity.h"
#include "cTransform.h"
#include "cVelocity.h"

#include <vector>

#include <glm/glm.hpp>

class Flock
{
public:
	Flock(void);
	Flock(float cohesionWeight, float separationWeight, float alignmentWeight);
	virtual ~Flock(void);

	void AddFlockMember(Entity* entity);
	void RemoveFlockMember(Entity* entity);

	void CalculateSteering(void);

private:
	struct FlockMemberInfo
	{
		FlockMemberInfo(Entity* entity, Transform* transform, Velocity* velocity);
		~FlockMemberInfo(void);

		Entity* entity;
		Transform* transform;
		Velocity* velocity;
	};

	void CalculateVectors(void);
	void GetSteeringFor(FlockMemberInfo* member, glm::vec3& flockSteering);

	std::vector<FlockMemberInfo*> mFlockMembers;

	glm::vec3 cohesion;
	glm::vec3 separation;
	glm::vec3 alignment;

	float cohesionWeight;
	float alignmentWeight;
	float separationWeight;
};

#endif
