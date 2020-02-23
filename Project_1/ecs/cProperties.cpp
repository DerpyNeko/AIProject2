#include "cProperties.h"

Properties::Properties()
{	
	this->materialDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->materialSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	this->bIsVisible = true;
	this->bIsWireFrame = false;

	this->bDontLight = false;
	this->bUseVertexColour = false;
}

void Properties::setDiffuseColour(glm::vec3 newDiffuse)
{
	this->materialDiffuse = glm::vec4(newDiffuse, this->materialDiffuse.a);
}