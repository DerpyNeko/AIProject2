#ifndef _HG_cTransform_
#define _HG_cTransform_

#define GLM_ENABLE_EXPERIMENTAL

#include "Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

class Transform : public Component
{
public:
	Transform();

	glm::vec3 position;
	glm::vec3 previousPosition;
	glm::vec3 scale;
	glm::quat orientation;
	float sphereRadius;

	void setUniformScale(float scale);
	glm::quat getQOrientation(void);
	void adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees/*=false*/);
};

extern const glm::vec3 UP;
extern const glm::vec3 RIGHT;
extern const glm::vec3 FORWARD;

#endif

