#include "bPathFollow.h"
#include "../globalStuff.h"

bool isReversed = false;
bool isBorked = false;

PathFollowBehaviour::PathFollowBehaviour(Entity* entity, Path* path)
	: mEntity(entity), mPath(path), mCurrentGoal(0), mPathRadius(15.0f)
{
}

PathFollowBehaviour::~PathFollowBehaviour(void)
{
}

void PathFollowBehaviour::Update(float dt)
{
	// Check if the entity is at the goal position
	Transform* agentTransform = mEntity->GetComponent<Transform>();
	Velocity* agentVelocity = mEntity->GetComponent<Velocity>();
	PathNode pathNode = mPath->pathNodes[mCurrentGoal];
	Properties* nodeProperties = 0;

	for (Entity* e : EntityManager::GetEntityList())
	{
		Properties* p = e->GetComponent<Properties>();
		Transform* t = e->GetComponent<Transform>();

		if (p->type == eType::OTHER)
		{
			if (t->position == pathNode.position)
			{
				nodeProperties = p;
			}
		}
	}

	nodeProperties->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));

	float dist = glm::distance(agentTransform->position, pathNode.position);

	if (dist < mPathRadius)
	{
		if (mCurrentGoal == mPath->pathNodes.size() - 1 || isReversed && mCurrentGoal == 0)
		{

			for (Entity* node : EntityManager::GetEntityList())
			{
				Properties* p = node->GetComponent<Properties>();

				if (p->type == eType::OTHER)
					p->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
			}

			agentVelocity->velocity = glm::vec3(0.001f, 0.001f, 0.0f);
			return;
		}

		nodeProperties->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
		isBorked = false;

		if (isReversed && mCurrentGoal != 0)
		{
			mCurrentGoal--;
		}
		else if (!isReversed)
		{
			mCurrentGoal++;
		}
	}

	glm::vec3 desiredVelocity = glm::normalize(mPath->pathNodes[mCurrentGoal].position - agentTransform->position);

	float magnitude = glm::length(pathNode.position - agentTransform->position);

	desiredVelocity *= MAXVELOCITY;

	//glm::vec3 steer;
	//steer = desiredVelocity - agentVelocity->velocity;

	//glm::quat orientation = glm::quat(glm::lookAt(agentTransform->position, agentTransform->position + steer, UP));

	//std::cout << orientation.z << std::endl;
	//orientation = glm::normalize(orientation);


	//agentTransform->orientation = orientation;	
	//agentTransform->adjMeshOrientationEulerAngles(glm::vec3(0, 0, orientation.z), true);

	//agentVelocity->velocity = steer * dt;

	glm::vec3 steer;
	steer.x = desiredVelocity.x - agentVelocity->velocity.x;
	steer.y = desiredVelocity.y - agentVelocity->velocity.y;

	glm::quat orientation = glm::quat(glm::lookAt(agentTransform->position, agentTransform->position + steer, UP));

	orientation = glm::normalize(orientation);

	//if (!isnan(orientation.z))
	if (!isBorked)
	{
		std::cout << orientation.z << std::endl;
		//agentTransform->orientation = orientation;

		//float dotProduct = glm::dot(agentTransform->position, pathNode.position);

		//float dotProduct = glm::dot(glm::vec3(4,3,0), glm::vec3(3,5,0));
		//std::cout << dotProduct << std::endl;
		//float magA = glm::length(glm::vec3(4, 3, 0));
		//float magB = glm::length(glm::vec3(3, 5, 0));

		//float magA = glm::length(agentTransform->position);
		//float magB = glm::length(pathNode.position);

		//std::cout << magA << " " << magB << std::endl;
		//float h = dotProduct / (magA * magB);
		//float num = acos(h) * 180/PI;
		//std::cout << num << std::endl;

		//float product = acos(dotProduct / (magA * magB)) * 180 / PI;

		//float angle = product - orientation.z;

		float angle = 0.0f;
		//agentTransform->orientation.z = orientation.z;
		if (desiredVelocity.y < agentVelocity->velocity.y)
			angle = atan(steer.x / steer.y);
		else
			angle = atan(steer.y / steer.x);

		agentTransform->adjMeshOrientationEulerAngles(glm::vec3(0, 0, angle), false);
		isBorked = true;
	}

	//agentTransform->adjMeshOrientationEulerAngles(glm::vec3(0, 0, orientation.z), true);
	//agentTransform->orientation = orientation;

	agentVelocity->velocity.x += steer.x * dt;
	agentVelocity->velocity.y += steer.y * dt;

	if (magnitude > MAXVELOCITY)
	{
		glm::vec3 normalized = agentVelocity->velocity;
		double mag = glm::length(normalized);
		normalized /= mag;

		agentVelocity->velocity = normalized * MAXVELOCITY;
	}
}

std::string PathFollowBehaviour::GetName(void)
{
	return "PathFollowBehaviour";
}
