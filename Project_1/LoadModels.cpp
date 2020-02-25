#include "globalOpenGLStuff.h"
#include "globalStuff.h"

#include "ecs/Behaviours.h"

Entity* g_player;
Entity* g_ring;

std::vector<Entity*> g_bullets;

// Loading models was moved into this function
void LoadModelTypes(cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID)
{
	sModelDrawInfo sphereInfo;
	sphereInfo.meshFileName = "Sphere_n_uv.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(sphereInfo, shaderProgramID);

	sModelDrawInfo spherePointInfo;
	spherePointInfo.meshFileName = "SpherePoint_n_uv.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(spherePointInfo, shaderProgramID);

	sModelDrawInfo sphereInvertedNormalsInfo;
	sphereInvertedNormalsInfo.meshFileName = "Sphere_n_uv_INVERTED_NORMALS.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(sphereInvertedNormalsInfo, shaderProgramID);

	sModelDrawInfo ringInfo;
	ringInfo.meshFileName = "Ring.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(ringInfo, shaderProgramID);

	// At this point, mesh in in GPU
	std::cout << "Mesh was loaded OK" << std::endl;

	// Load the cube map
	::g_pTextureManager->SetBasePath("assets/textures/cubemaps");
	std::string errorString;
	if (::g_pTextureManager->CreateCubeTextureFromBMPFiles("CityCubeMap",
		"TropicalSunnyDayLeft2048.bmp", "TropicalSunnyDayRight2048.bmp",		// Alternate these
		"TropicalSunnyDayDown2048.bmp", "TropicalSunnyDayUp2048.bmp", 			// Rotate these 90 degrees
		"TropicalSunnyDayFront2048.bmp", "TropicalSunnyDayBack2048.bmp", true, errorString))
	{
		std::cout << "Loaded the city cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: city cube map DIDN't load. On no!" << std::endl;
	}

	return;
}

// Loads the models we are drawing into the vector
void LoadModelsIntoScene()
{
	// ENTITY #0 - Skybox
	{
		Entity* entity = EntityManager::CreateEntity();
		entity->name = "SkyBoxObject";

		Properties* properties = entity->AddComponent<Properties>();
		properties->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
		properties->meshName = "Sphere_n_uv_INVERTED_NORMALS.ply";
		properties->bIsVisible = false;

		Transform* transform = entity->AddComponent<Transform>();
		transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
		transform->setUniformScale(5000.0f);
		transform->orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		transform->sphereRadius *= transform->scale.x;
	}

	// ENTITY #1 - The Player
	{
		g_player = EntityManager::CreateEntity();
		g_player->name = "Player";

		Properties* properties1 = g_player->AddComponent<Properties>();
		properties1->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
		properties1->bDontLight = true;
		properties1->meshName = "SpherePoint_n_uv.ply";
		properties1->type = eType::PLAYER;

		Transform* playerTransform = g_player->AddComponent<Transform>();
		playerTransform->position = glm::vec3(0.0f, 0.0f, 0.0f);
		playerTransform->setUniformScale(15.0f);
		playerTransform->orientation = glm::quat(0.0f, 0.0f, 1.0f, 0.0f);
		playerTransform->sphereRadius = playerTransform->scale.x;

		g_player->AddComponent<Velocity>();
	}

	// ENTITY #2 - Player Radius
	{
		g_ring = EntityManager::CreateEntity();
		g_ring->name = "Ring";

		Properties* properties1 = g_ring->AddComponent<Properties>();
		properties1->setDiffuseColour(glm::vec3(0.0f, 1.0f, 1.0f));
		properties1->bDontLight = true;
		properties1->meshName = "Ring.ply";
		properties1->type = eType::PLAYER;

		Transform* transform1 = g_ring->AddComponent<Transform>();
		transform1->position = glm::vec3(0.0f, 0.0f, 0.0f);
		transform1->setUniformScale(0.5f);
		transform1->orientation = glm::quat(0.0f, 0.0f, 1.0f, 0.0f);
		transform1->sphereRadius = transform1->scale.x;

		g_ring->AddComponent<Velocity>();
	}

	// ENTITY #3-15 - Entities
	for (int i = 0; i < 12; i++)
	{
		Entity* entity2 = EntityManager::CreateEntity();
		entity2->name = "Entity" + std::to_string(i);

		Properties* properties2 = entity2->AddComponent<Properties>();
		properties2->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
		properties2->bDontLight = true;
		properties2->meshName = "SpherePoint_n_uv.ply";
		properties2->type = eType::ENEMY;

		float x = ((float(rand()) / float(RAND_MAX)) * (445 - (-445))) + (-445);
		float y = ((float(rand()) / float(RAND_MAX)) * (445 - (-445))) + (-445);

		Transform* transform2 = entity2->AddComponent<Transform>();
		transform2->position = glm::vec3(x, y, 0.0f);
		transform2->setUniformScale(15.0f);
		transform2->orientation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
		transform2->sphereRadius = transform2->scale.x;

		Velocity* velocity = entity2->AddComponent<Velocity>();
		velocity->velocity = glm::normalize(glm::vec3(sin(rand()), sin(rand()), 0.0f));
	}

	// ENTITY #16-21 - Path Nodes
	for (int i = 0; i < 5; i++)
	{
		Entity* entity2 = EntityManager::CreateEntity();
		entity2->name = "PathNode" + std::to_string(i);

		Properties* properties2 = entity2->AddComponent<Properties>();
		properties2->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
		properties2->bDontLight = true;
		properties2->meshName = "Sphere_n_uv.ply";
		properties2->type = eType::OTHER;
		properties2->bIsWireFrame = true;

		int randomNum = rand() % 891 + (-445);
		float x = ((float(rand()) / float(RAND_MAX)) * (445 - (-445))) + (-445);
		float y = ((float(rand()) / float(RAND_MAX)) * (445 - (-445))) + (-445);

		Transform* transform2 = entity2->AddComponent<Transform>();
		transform2->position = glm::vec3(x, y, 0.0f);
		transform2->setUniformScale(15.0f);
		transform2->orientation = glm::quat(0.0f, 1.0f, 0.0f, 0.0f);
		transform2->sphereRadius = transform2->scale.x;

		entity2->AddComponent<Velocity>();
	}

	// ENTITY #22 - Debug Sphere
	{
		Entity* entity = EntityManager::CreateEntity();
		entity->name = "DebugSphere";
		Properties* properties = entity->AddComponent<Properties>();
		properties->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
		properties->meshName = "Sphere_n_uv.ply";
		properties->bIsVisible = false;
		properties->bIsWireFrame = true;
		Transform* transform = entity->AddComponent<Transform>();
		transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
		transform->setUniformScale(0.1f);
	}

	return;
}