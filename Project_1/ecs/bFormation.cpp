#include "bFormation.h"

#include "EntityManager.h"
#include "cTransform.h"
#include "cVelocity.h"
#include "cProperties.h"

// Vectors of formation offsets based on the black player position
std::vector<glm::vec3> circleFormation = {
	glm::vec3(0.0f, -100.0f, 0.0f),		glm::vec3(75.0f, -75.0f, 0.0f),
	glm::vec3(-75.0f, -75.0f, 0.0f),	glm::vec3(25.0f, -25.0f, 0.0f),
	glm::vec3(-25.0f, -25.f, 0.0f),		glm::vec3(100.0f, 0.0f, 0.0f),
	glm::vec3(-100.0f, 0.0f, 0.0f),		glm::vec3(25.0f, 25.0f, 0.0f),
	glm::vec3(-25.0f, 25.0f, 0.0f),		glm::vec3(75.0f, 75.0f, 0.0f),
	glm::vec3(-75.0f, 75.0f, 0.0f),		glm::vec3(0.0f, 100.0f, 0.0f)
};

std::vector<glm::vec3> vFormation = {
	glm::vec3(-25.0f, -25.0f, 0.0f),	glm::vec3(25.0f, -25.0f, 0.0f),
	glm::vec3(-50.0f, -50.0f, 0.0f),	glm::vec3(50.0f, -50.0f, 0.0f),
	glm::vec3(-75.0f, -75.0f, 0.0f),	glm::vec3(75.0f, -75.0f, 0.0f),
	glm::vec3(-100.0f, -100.0f, 0.0f),	glm::vec3(100.0f, -100.0f, 0.0f),
	glm::vec3(-125.0f, -125.0f, 0.0f),	glm::vec3(125.0f, -125.0f, 0.0f),
	glm::vec3(-150.0f, -150.0f, 0.0f),	glm::vec3(150.0f, -150.0f, 0.0f)
};

std::vector<glm::vec3> squareFormation = {
	glm::vec3(-100.0f, -100.0f, 0.0f),	glm::vec3(50.0f, -100.0f, 0.0f),
	glm::vec3(-50.0f, -100.0f, 0.0f),	glm::vec3(100.0f, -100.0f, 0.0f),
	glm::vec3(-100.0f, -50.f, 0.0f),	glm::vec3(100.0f, -50.0f, 0.0f),
	glm::vec3(-100.0f, 50.0f, 0.0f),	glm::vec3(100.0f, 50.0f, 0.0f),
	glm::vec3(-100.0f, 100.0f, 0.0f),	glm::vec3(50.0f, 100.0f, 0.0f),
	glm::vec3(-50.0f, 100.0f, 0.0f),	glm::vec3(100.0f, 100.0f, 0.0f)
};

std::vector<glm::vec3> lineFormation = {
	glm::vec3(-300.0f, 0.0f, 0.0f),		glm::vec3(-250.0f, 0.0f, 0.0f),
	glm::vec3(-200.0f, 0.0f, 0.0f),		glm::vec3(-150.0f, 0.0f, 0.0f),
	glm::vec3(-100.0f, 0.0f, 0.0f),		glm::vec3(-50.0f, 0.0f, 0.0f),
	glm::vec3(50.0f, 0.0f, 0.0f),		glm::vec3(100.0f, 0.0f, 0.0f),
	glm::vec3(150.0f, 0.0f, 0.0f),		glm::vec3(200.0f, 0.0f, 0.0f),
	glm::vec3(250.0f, 0.0f, 0.0f),		glm::vec3(300.0f, 0.0f, 0.0f)
};

std::vector<glm::vec3> rowsFormation = {
	glm::vec3(-125.0f, -70.0f, 0.0f),	glm::vec3(-75.0f, -70.0f, 0.0f),
	glm::vec3(-25.0f, -70.0f, 0.0f),	glm::vec3(25.0f, -70.0f, 0.0f),
	glm::vec3(75.0f, -70.0f, 0.0f),		glm::vec3(125.0f, -70.0f, 0.0f),
	glm::vec3(-125.0f, 70.0f, 0.0f),	glm::vec3(-75.0f, 70.0f, 0.0f),
	glm::vec3(-25.0f, 70.0f, 0.0f),		glm::vec3(25.0f, 70.0f, 0.0f),
	glm::vec3(75.0f, 70.0f, 0.0f),		glm::vec3(125.0f, 70.0f, 0.0f)
};
FormationBehaviour::FormationBehaviour(std::vector<glm::vec3> formation) 
	: mCurrentFormation(formation)
{
}

FormationBehaviour::~FormationBehaviour(void)
{
}

void FormationBehaviour::Update(float dt)
{
	int index = 0;

	for (Entity* e : EntityManager::GetEntityList())
	{
		Properties* p = e->GetComponent<Properties>();

		if (p->type == eType::ENEMY)
		{
			Transform* agentTransform = e->GetComponent<Transform>();
			Velocity* agentVelocity = e->GetComponent<Velocity>();
			
			Transform* playerTransform = g_player->GetComponent<Transform>();

			glm::vec3 targetPosition = playerTransform->position + mCurrentFormation[index];

			glm::vec3 desiredVelocity = glm::normalize(targetPosition - agentTransform->position);

			float magnitude = glm::length(targetPosition - agentTransform->position);

			desiredVelocity *= MAXVELOCITY;

			glm::vec3 steer;
			steer = desiredVelocity - agentVelocity->velocity;


			if (steer.x == 0 && steer.y == 0 && steer.z == 0 || magnitude < 1.0f)
			{
				agentVelocity->velocity = glm::vec3(0, 0, 0);
				index++;
				continue;
			}

			glm::quat orientation = glm::quat(glm::lookAt(agentTransform->position, agentTransform->position + steer, UP));

			orientation = glm::normalize(orientation);

			agentTransform->orientation = orientation;

			//float angle = 0.0f;
			//if (desiredVelocity.y < agentVelocity->velocity.y)
			//	angle = atan(steer.x / steer.y);
			//else
			//	angle = atan(steer.y / steer.x);

			//agentTransform->adjMeshOrientationEulerAngles(glm::vec3(0, 0, angle), false);

			agentVelocity->velocity = steer * dt;

			if (magnitude > MAXVELOCITY)
			{
				glm::vec3 normalized = agentVelocity->velocity;
				double mag = glm::length(normalized);
				normalized /= mag;

				agentVelocity->velocity = normalized * MAXVELOCITY;
			}

			index++;
		}
	}
}

std::string FormationBehaviour::GetName()
{
	return "FormationBehaviour";
}
