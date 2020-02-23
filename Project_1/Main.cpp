#include <iostream>
#include <ctime>
#include "ecs/Engine.h"
#include "ecs/sMotionSystem.h"
#include "ecs/sCollisionSystem.h"

int main(void)
{
	int result;
	srand(time(0));

	printf("Initializing engine...\n");
	result = Engine::Initialize();
	if (result != 0) {
		printf("Failed to initialize engine with error code %d\n", result);
		return 1;
	}

	Engine::AddSystem(new MotionSystem());
	Engine::AddSystem(new CollisionSystem());

	printf("Running...\n");
	result = Engine::Run();
	if (result != 0) {
		printf("Error running the engine with error code %d\n", result);
		return 1;
	}

	printf("Shutting down the engine...\n");
	result = Engine::Destroy();
	if (result != 0) {
		printf("Failed to shut down the engine with error code %d\n", result);
		return 1;
	}

	system("pause");

	return 0;
}
