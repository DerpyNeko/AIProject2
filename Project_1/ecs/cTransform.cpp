#include "cTransform.h"

const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0);
const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0);
const glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, 1.0);

Transform::Transform()
{
	this->position = glm::vec3(0.0f);
	this->previousPosition = glm::vec3(0.0f);
	this->scale = glm::vec3(1.0f);
	this->orientation = glm::quat((glm::vec3(0.0f, 0.0f, 0.0f)));
	this->sphereRadius = 1.0f;
}

void Transform::setUniformScale(float scale)
{
	this->scale = glm::vec3(scale, scale, scale);
}

glm::quat Transform::getQOrientation(void) 
{ 
	return this->orientation;
};

void Transform::adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees/*=false*/) {
	if (bIsDegrees) {
		adjAngleEuler = glm::vec3(glm::radians(adjAngleEuler.x), glm::radians(adjAngleEuler.y), glm::radians(adjAngleEuler.z));
	}

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust(adjAngleEuler);

	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->orientation *= rotationAdjust;
	return;
}
