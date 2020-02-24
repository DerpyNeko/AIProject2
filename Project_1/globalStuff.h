#ifndef _globalStuff_HG_
#define _globalStuff_HG_

#include "globalOpenGLStuff.h"

#include "ecs/Entity.h"
#include "ecs/cProperties.h"
#include "ecs/cTransform.h"
#include "ecs/cVelocity.h"
#include "ecs/BehaviourManager.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <string>
#include <iostream>
#include <vector>

#include "sPlyVertex.h"
#include "sPlyTriangle.h"
#include "sModelDrawInfo.h"
#include "cVAOMeshManager.h"

#include "cLightManager.h"
#include "cCamera.h"
#include "cBasicTextureManager.h"

#include "CErrorLog.h"
#include "iDebugRenderer.h"

extern iDebugRenderer* g_pDebugRenderer;

extern cCamera* g_pCamera;

extern cVAOMeshManager* g_pVAOMeshManager;
extern cBasicTextureManager* g_pTextureManager;

extern std::vector<sLight*> g_Lights;
extern int g_LightIndex;

extern bool g_bDrawDebugLightSpheres;

void CreateLights(GLint program);

void LoadModelTypes(cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID);
void LoadModelsIntoScene();

void DrawObject(Entity* entity, glm::mat4x4& matModel, GLuint shaderProgramID);

extern BehaviourManager gBehaviourManager;
extern double startTime;

extern Entity* g_player;

extern std::vector<Entity*> g_bullets;

extern std::vector<Entity*> g_PathNodes;

extern bool isStopped;
extern bool isReversed;

#endif	// _globalStuff_HG_
