#include "bFlock.h"
#include "cProperties.h"

FlockBehaviour::FlockMemberInfo::FlockMemberInfo(Entity* entity, Transform* transform, Velocity* velocity)
	: entity(entity)
	, transform(transform)
	, velocity(velocity)
{
}

FlockBehaviour::FlockMemberInfo::~FlockMemberInfo(void)
{
}

FlockBehaviour::FlockBehaviour(Entity* entity)
	: cohesionWeight(1.0f / 3.0f)
	, separationWeight(1.0f / 3.0f)
	, alignmentWeight(1.0f / 3.0f)
{
	cohesion = glm::vec3(0.0f);
	separation = glm::vec3(0.0f);
	alignment = glm::vec3(0.0f);

	AddFlockMember(entity);
}

FlockBehaviour::FlockBehaviour(Entity* entity, float cohesionWeight, float separationWeight, float alignmentWeight)
	: cohesionWeight(cohesionWeight)
	, separationWeight(separationWeight)
	, alignmentWeight(alignmentWeight)
{
	cohesion = glm::vec3(0.0f);
	separation = glm::vec3(0.0f);
	alignment = glm::vec3(0.0f);

	AddFlockMember(entity);
}

FlockBehaviour::~FlockBehaviour(void)
{
}

void FlockBehaviour::Update(float dt)
{
	int radius = 100;

	Transform* playerTransform = mFlockMembers[0]->entity->GetComponent<Transform>();

	for (Entity* e : EntityManager::GetEntityList())
	{
		Properties* properties = e->GetComponent<Properties>();

		if (properties->type == eType::ENEMY)
		{
			Transform* transform = e->GetComponent<Transform>();

			if ((transform->position.x <= playerTransform->position.x + radius &&
				transform->position.x >= playerTransform->position.x - radius) &&
				(transform->position.y <= playerTransform->position.y + radius &&
					transform->position.y >= playerTransform->position.y - radius))
			{
				AddFlockMember(e);
				properties->bIsPartOfFlock = true;
			}

			if (transform->position.x >= playerTransform->position.x + radius ||
				transform->position.x <= playerTransform->position.x - radius ||
				transform->position.y >= playerTransform->position.y + radius ||
				transform->position.y <= playerTransform->position.y - radius)
			{
				if (properties->bIsPartOfFlock == true)
				{
					RemoveFlockMember(e);
					properties->bIsPartOfFlock = false;
				}
			}
		}
	}

	CalculateSteering();
}

std::string FlockBehaviour::GetName()
{
	return "FlockBehaviour";
}

void FlockBehaviour::AddFlockMember(Entity* entity)
{
	Properties* properties = entity->GetComponent<Properties>();
	if (properties->bIsPartOfFlock)
		return;

	std::cout << "Adding flock member" << std::endl;

	Transform* transform = entity->GetComponent<Transform>();
	Velocity* velocity = entity->GetComponent<Velocity>();

	FlockMemberInfo* flockMemberInfo = new FlockMemberInfo(entity, transform, velocity);
	mFlockMembers.push_back(flockMemberInfo);

	if (entity->name != "Player")
	{
		properties->setDiffuseColour(glm::vec3(1.0f, 0.0f, 1.0f));
	}
}

void FlockBehaviour::RemoveFlockMember(Entity* entity)
{
	for (int i = 0; i < mFlockMembers.size(); i++)
	{
		if (mFlockMembers[i]->entity == entity)
		{
			std::cout << "Removing flock member" << std::endl;

			mFlockMembers.erase(mFlockMembers.begin() + i);
		}
	}

	Properties* properties = entity->GetComponent<Properties>();
	properties->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));

	Velocity* velocity = entity->GetComponent<Velocity>();
	velocity->velocity = glm::normalize(glm::vec3(sin(rand()), sin(rand()), 0.0f));
}

void FlockBehaviour::CalculateVectors(void)
{
	glm::vec3 forward;

	cohesion = glm::vec3(0.0f);
	separation = glm::vec3(0.0f);
	alignment = glm::vec3(0.0f);
	for (std::vector<FlockMemberInfo*>::iterator itMember = mFlockMembers.begin();
		itMember != mFlockMembers.end(); itMember++)
	{
		FlockMemberInfo* currMember = *itMember;

		if (currMember->velocity->velocity.length != 0)
		{
			forward = glm::normalize(currMember->velocity->velocity);
		}

		if (currMember->velocity->velocity != currMember->velocity->velocity)
		{
			currMember->velocity->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		cohesion += currMember->transform->position;
		separation += currMember->transform->position;
		alignment += forward;
	}
}

void FlockBehaviour::CalculateSteering(void)
{
	CalculateVectors();

	glm::vec3 steering;

	if (mFlockMembers.size() > 1)
	{
		for (std::vector<FlockMemberInfo*>::iterator itMember = mFlockMembers.begin();
			itMember != mFlockMembers.end(); itMember++)
		{
			GetSteeringFor(*itMember, steering);

			(*itMember)->velocity->acceleration = steering;
		}
	}
}

void FlockBehaviour::GetSteeringFor(FlockMemberInfo* member, glm::vec3& flockSteering)
{
	glm::vec3 forward;
	if (member->velocity->velocity.length != 0)
	{
		forward = glm::normalize(member->velocity->velocity);
	}
	else
	{
		forward = member->velocity->velocity;
	}

	float sizeMinusOne = (float)(mFlockMembers.size() - 1);

	if (mFlockMembers.size() < 2)
	{
		flockSteering = forward;
	}

	// Calcuale cohesion vector
	glm::vec3 cohesionVec = glm::normalize((cohesion - member->transform->position) / sizeMinusOne - member->transform->position) * cohesionWeight;

	// Remove the current position from the separation vector to exclude the 
	// current flock member from the sum of positions.
	//glm::vec3 separationVec = (separation - member->transform->position);
	//separationVec /= sizeMinusOne;
	//separationVec = -glm::normalize(member->transform->position - separationVec) * separationWeight;
	//separationVec = glm::normalize(cohesion - member->transform->position);

	glm::vec3 separationVec = glm::vec3(0);
	for (FlockMemberInfo* f : mFlockMembers)
	{
		if (glm::distance(member->transform->position, f->transform->position) != 0)
		{
			separationVec += glm::normalize(member->transform->position - f->transform->position);
		}
	}

	if (separationVec.length != 0) {
		separationVec = -normalize(separationVec);
	}

	// Sum of vectors of other boid positions to current boid normalized

	// Remove the current alignment from the separation vector to exclude the
	// current flock member from the sum of alignments;
	glm::vec3 alignmentVec = (alignment - forward);
	alignmentVec /= sizeMinusOne;
	alignmentVec = glm::normalize(alignmentVec) * alignmentWeight;

#ifdef LOG_SYSTEMS
	printf("CohesionVec Mag=%0.4f\n", glm::length(cohesionVec));
	printf("separationVec Mag=%0.4f\n", glm::length(separationVec));
	printf("alignmentVec Mag=%0.4f\n", glm::length(alignmentVec));
#endif

	// calculate the final steering direction
	// These three vectors should add up to a normalized vector, assuming
	// the weight values add up to 1.
	flockSteering = cohesionVec + separationVec + alignmentVec;

	if (glm::length(flockSteering) == 0)
	{
		flockSteering = forward;
	}

	flockSteering = glm::normalize(flockSteering);
}
