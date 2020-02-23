#include "globalOpenGLStuff.h"
#include "globalStuff.h"

bool isWPressed = false;
bool isSPressed = false;

int bulletCount = -1;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (bulletCount >= 9)
			bulletCount = 0;

		bulletCount++;

		Transform* bulletTransform = g_bullets.at(bulletCount)->GetComponent<Transform>();
		Velocity* bulletVelocity = g_bullets.at(bulletCount)->GetComponent<Velocity>();

		bulletTransform->position = playerTransform->position;
		bulletVelocity->vx = playerVelocity->vx * 1.4f;
		bulletVelocity->vy = playerVelocity->vy * 1.4f;
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		std::cout << playerTransform->position.x << " " << playerTransform->position.y << " " << playerTransform->position.z << std::endl;
	}

	return;
}

void ProcessAsyncKeys(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)	// "fowards"
	{
		playerVelocity->vy = (glm::vec3(0, 3.0f, 0) * glm::toMat3(playerTransform->orientation)).y;
		playerVelocity->vx = (glm::vec3(0, -3.0f, 0) * glm::toMat3(playerTransform->orientation)).x;
		isWPressed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && isWPressed == true)
	{
		playerVelocity->vx = 0.0f;
		playerVelocity->vy = 0.0f;
		isWPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	// "backwards"
	{
		playerVelocity->vy = (glm::vec3(0, -3.0f, 0) * glm::toMat3(playerTransform->orientation)).y;
		playerVelocity->vx = (glm::vec3(0, 3.0f, 0) * glm::toMat3(playerTransform->orientation)).x;
		isSPressed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE && isSPressed == true)
	{
		playerVelocity->vx = 0.0f;
		playerVelocity->vy = 0.0f;
		isSPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	// rotate "left"
	{
		playerTransform->adjMeshOrientationEulerAngles(glm::vec3(0, 0, -3), true);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	// rotate "right"
	{
		playerTransform->adjMeshOrientationEulerAngles(glm::vec3(0, 0, 3), true);
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
