#include "globalOpenGLStuff.h"
#include "globalStuff.h"
#include "ecs/bPathFollow.h"
#include "ecs/EntityManager.h"

bool isWPressed = false;
bool isSPressed = false;

int bulletCount = -1;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Transform* playerTransform = g_player->GetComponent<Transform>();
	Velocity* playerVelocity = g_player->GetComponent<Velocity>();

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	//{
	//	if (bulletCount >= 9)
	//		bulletCount = 0;

	//	bulletCount++;

	//	Transform* bulletTransform = g_bullets.at(bulletCount)->GetComponent<Transform>();
	//	Velocity* bulletVelocity = g_bullets.at(bulletCount)->GetComponent<Velocity>();

	//	bulletTransform->position = playerTransform->position;
	//	bulletVelocity->velocity = playerVelocity->velocity * 1.4f;
	//}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		std::cout << playerTransform->position.x << " " << playerTransform->position.y << std::endl;
	}

	//if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
	//{
	//	isFormation = false;
	//	isFlock = false;
	//}

	//if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	//{
	//	formation = circleFormation;
	//	isFormation = true;
	//	isFlock = false;
	//}

	//if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	//{
	//	formation = vFormation;
	//	isFormation = true;
	//	isFlock = false;
	//}

	//if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	//{
	//	formation = squareFormation;
	//	isFormation = true;
	//	isFlock = false;
	//}

	//if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	//{
	//	formation = lineFormation;
	//	isFormation = true;
	//	isFlock = false;
	//}

	//if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	//{
	//	formation = rowsFormation;
	//	isFormation = true;
	//	isFlock = false;
	//}

	//if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	//{
	//	isFlock = !isFlock;
	//	isFormation = false;
	//}

	//if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	//{
	//	isFlock = false;
	//	isFormation = true;
	//}

	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	{
		Path* path = new Path();
		for (Entity* e : EntityManager::GetEntityList())
		{
			Properties* p = e->GetComponent<Properties>();
			Transform* t = e->GetComponent<Transform>();
			if (p->type == eType::OTHER)
			{
				path->pathNodes.push_back(PathNode(t->position));
			}
		}
		//path->pathNodes.push_back(PathNode(glm::vec3(235.0f, 540.0f, 0.0f)));
		//path->pathNodes.push_back(PathNode(glm::vec3(20.0f, -15.0f, 0.0f)));
		//path->pathNodes.push_back(PathNode(glm::vec3(-220.0f, 235.0f, 0.0f)));
		//path->pathNodes.push_back(PathNode(glm::vec3(-735.0f, 410.0f, 0.0f)));
		//path->pathNodes.push_back(PathNode(glm::vec3(-1045.0f, -90.0f, 0.0f)));
		gBehaviourManager.SetBehaviour(g_player, new PathFollowingBehaviour(g_player, path));
	}

	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
	{
		isReversed = !isReversed;
		isStopped = false;

		std::cout << "Key 9: isReversed: " << isReversed << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		isStopped = !isStopped;
	}

	return;
}

void ProcessAsyncKeys(GLFWwindow* window)
{
	Transform* playerTransform = g_player->GetComponent<Transform>();
	Velocity* playerVelocity = g_player->GetComponent<Velocity>();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)	// "fowards"
	{
		playerVelocity->velocity.y = (glm::vec3(0, 2.0f, 0) * glm::toMat3(playerTransform->orientation)).y;
		playerVelocity->velocity.x = (glm::vec3(0, 2.0f, 0) * glm::toMat3(playerTransform->orientation)).x;
		isWPressed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && isWPressed == true)
	{
		playerVelocity->velocity = glm::vec3(0, 0, 0);
		isWPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	// "backwards"
	{
		playerVelocity->velocity.y = (glm::vec3(0, -2.0f, 0) * glm::toMat3(playerTransform->orientation)).y;
		playerVelocity->velocity.x = (glm::vec3(0, -2.0f, 0) * glm::toMat3(playerTransform->orientation)).x;
		isSPressed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE && isSPressed == true)
	{
		playerVelocity->velocity = glm::vec3(0, 0, 0);
		isSPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	// rotate "left"
	{
		playerTransform->adjMeshOrientationEulerAngles(glm::vec3(0, 0, 3), true);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	// rotate "right"
	{
		playerTransform->adjMeshOrientationEulerAngles(glm::vec3(0, 0, -3), true);
	}

	return;
}

bool g_MouseIsInsideWindow = false;

void cursor_enter_callback(GLFWwindow* window, int entered)
{
	if (entered)
	{
		::g_MouseIsInsideWindow = true;
	}
	else
	{
		::g_MouseIsInsideWindow = false;
	}
	return;
}

// Mouse (cursor) callback
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	return;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	return;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// A regular mouse wheel returns the y value
	::g_pCamera->setMouseWheelDelta(yoffset);

	return;
}

void ProcessAsyncMouse(GLFWwindow* window)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	::g_pCamera->setMouseXY(x, y);

	const float MOUSE_SENSITIVITY = 0.1f;

	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && ::g_MouseIsInsideWindow)
	{
		::g_pCamera->Yaw_LeftRight(-::g_pCamera->getDeltaMouseX() * MOUSE_SENSITIVITY);
		::g_pCamera->Pitch_UpDown(::g_pCamera->getDeltaMouseY() * MOUSE_SENSITIVITY);
	}

	// Adjust the movement speed based on the wheel position
	if (::g_MouseIsInsideWindow)
	{
		::g_pCamera->movementSpeed = ::g_pCamera->getMouseWheel() * MOUSE_SENSITIVITY;
	}

	return;
}
