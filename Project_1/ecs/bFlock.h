#ifndef _HG_FlockBehaviour_
#define _HG_FlockBehaviour_

#include "Behaviour.h"

#include <glm/glm.hpp>

class FlockBehaviour : public Behaviour
{
public:
	FlockBehaviour(Entity* entity);
	FlockBehaviour(Entity* entity, float cohesionWeight, float separationWeight, float alignmentWeight);

	virtual ~FlockBehaviour(void);
	virtual void Update(float dt);
	virtual std::string GetName(void);

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
	void GetFlockMembers(std::vector<FlockMemberInfo*> vector);

	std::vector<FlockMemberInfo*> mFlockMembers;

	glm::vec3 cohesion;
	glm::vec3 separation;
	glm::vec3 alignment;

	float cohesionWeight;
	float alignmentWeight;
	float separationWeight;
};

#endif
