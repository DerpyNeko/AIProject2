#include "Engine.h"
#include "EntityManager.h"

#include "../globalOpenGLStuff.h"
#include "../globalStuff.h"

#include "../cShaderManager.h"
#include "../cDebugRenderer.h"
#include "../cLightHelper.h"

#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

cDebugRenderer* g_pDebugRendererACTUAL = NULL;
iDebugRenderer* g_pDebugRenderer = NULL;

cShaderManager* pShaderManager = NULL;
cVAOMeshManager* g_pVAOMeshManager = NULL;
cBasicTextureManager* g_pTextureManager = NULL;

cCamera* g_pCamera = NULL;

std::vector<sLight*> g_Lights;
cLightHelper* pLightHelper;

GLFWwindow* window;

GLuint program;
GLint matView_location;
GLint matProj_location;
GLint eyeLocation_location;

bool g_bDrawDebugLightSpheres = false;
void DrawDebugLightSpheres(cLightHelper* pLightHelper, sLight* light, Entity* pDebugSphere, glm::mat4 matBall, GLuint program, glm::vec4 oldDiffuse, glm::vec3 oldScale);

void Update(void);
void Draw(void);

BehaviourManager gBehaviourManager;

std::vector<System*> gSystems;		// Container of Systems
double startTime;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

/**
 * Initialize
 * Acts as constructor for the Engine
 * @return - engine status code
 */
int Engine::Initialize(void)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(900, 900, "Project 1", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	// Mouse callbacks
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	// Create the shader manager...
	pShaderManager = new cShaderManager();
	pShaderManager->setBasePath("assets/shaders/");

	cShaderManager::cShader vertexShader;
	cShaderManager::cShader fragmentShader;

	vertexShader.fileName = "vertex01.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShader.fileName = "fragment01.glsl";
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (pShaderManager->createProgramFromFile("BasicUberShader", vertexShader, fragmentShader))
	{
		std::cout << "Compiled shaders OK." << std::endl;
	}
	else
	{
		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pShaderManager->getLastError() << std::endl;
	}

	// Load the uniform location values (some of them, anyway)
	cShaderManager::cShaderProgram* pSP = ::pShaderManager->pGetShaderProgramFromFriendlyName("BasicUberShader");
	pSP->LoadUniformLocation("texture00");
	pSP->LoadUniformLocation("texture01");
	pSP->LoadUniformLocation("texture02");
	pSP->LoadUniformLocation("texture03");
	pSP->LoadUniformLocation("texture04");
	pSP->LoadUniformLocation("texture05");
	pSP->LoadUniformLocation("texture06");
	pSP->LoadUniformLocation("texture07");
	pSP->LoadUniformLocation("texBlendWeights[0]");
	pSP->LoadUniformLocation("texBlendWeights[1]");

	program = pShaderManager->getIDFromFriendlyName("BasicUberShader");

	::g_pVAOMeshManager = new cVAOMeshManager();
	::g_pTextureManager = new cBasicTextureManager();

	// Loading the uniform variables here (rather than the inner draw loop)
	//GLint objectColour_UniLoc = glGetUniformLocation(program, "objectColour");

	//GLint matModel_location = glGetUniformLocation(program, "matModel");
	matView_location = glGetUniformLocation(program, "matView");
	matProj_location = glGetUniformLocation(program, "matProj");
	eyeLocation_location = glGetUniformLocation(program, "eyeLocation");

	// Note that this point is to the +interface+ but we're creating the actual object
	::g_pDebugRendererACTUAL = new cDebugRenderer();
	::g_pDebugRenderer = (iDebugRenderer*)::g_pDebugRendererACTUAL;

	if (!::g_pDebugRendererACTUAL->initialize())
	{
		std::cout << "Warning: couldn't init the debug renderer." << std::endl;
		std::cout << "\t" << ::g_pDebugRendererACTUAL->getLastError() << std::endl;
	}
	else
	{
		std::cout << "Debug renderer is OK" << std::endl;
	}

	::g_pCamera = new cCamera();

	CreateLights(program);
	pLightHelper = new cLightHelper();

	LoadModelTypes(::g_pVAOMeshManager, program);
	LoadModelsIntoScene();

	srand(time(NULL));

	return 0;
}

/**
* Destroy
* Acts as destructor for the Engine
* @return - engine status code
*/
int Engine::Destroy(void)
{
	unsigned int numSystems = gSystems.size();
	for (unsigned int i = 0; i < numSystems; i++)
	{
		delete gSystems[i];
	}
	gSystems.clear();

	delete pShaderManager;
	delete ::g_pVAOMeshManager;
	delete ::g_pTextureManager;

	delete ::g_pDebugRenderer;
	delete ::g_pCamera;

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}

/**
 * AddSystem
 * Adds a system to the engine
 */
int Engine::AddSystem(System* system)
{
	assert(system);
	gSystems.push_back(system);

	return 0;
}

/**
* Run
* The game loop
* @return - engine status code
*/
int Engine::Run(void)
{
	// Get the current time to start with
	startTime = glfwGetTime();

	// Draw the "scene" (run the program)
	while (!glfwWindowShouldClose(window))
	{
		// Switch to the shader we want
		::pShaderManager->useShaderProgram("BasicUberShader");

		float ratio;
		int width, height;

		glm::mat4x4 matProjection = glm::mat4(1.0f);
		glm::mat4x4	matView = glm::mat4(1.0f);

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH);			// Enables the KEEPING of the depth information
		glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
		glEnable(GL_CULL_FACE);		// Discared "back facing" triangles

		// Colour and depth buffers are TWO DIFF THINGS.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// FOV, Aspect ratio, Near clipping plane, Far clipping plane
		matProjection = glm::perspective(0.6f, ratio, 0.1f, 10000.0f);

		matView = glm::lookAt(::g_pCamera->eye, ::g_pCamera->getAtInWorldSpace(), ::g_pCamera->getUpVector());

		glUniform3f(eyeLocation_location, ::g_pCamera->eye.x, ::g_pCamera->eye.y, ::g_pCamera->eye.z);

		glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));

		for (sLight* light : g_Lights)
		{
			glUniform4f(light->position_UniLoc, light->position.x,
				light->position.y, light->position.z, 1.0f);

			glUniform4f(light->diffuse_UniLoc, light->diffuse.x,
				light->diffuse.y, light->diffuse.z, 1.0f);

			glUniform4f(light->param2_UniLoc, 1.0f, 0.0f, 0.0f, 0.0f);

			glUniform4f(light->atten_UniLoc, light->atten.x,
				light->atten.y, light->atten.z, light->atten.w);

			Entity* pDebugSphere = EntityManager::FindEntity("DebugSphere");
			Properties* sphereProperties = pDebugSphere->GetComponent<Properties>();
			Transform* sphereTransform = pDebugSphere->GetComponent<Transform>();

			sphereProperties->bIsVisible = true;
			sphereProperties->bDontLight = true;

			glm::vec4 oldDiffuse = sphereProperties->materialDiffuse;
			glm::vec3 oldScale = sphereTransform->scale;

			sphereProperties->setDiffuseColour(glm::vec3(255.0f / 255.0f, 105.0f / 255.0f, 180.0f / 255.0f));
			sphereProperties->bUseVertexColour = false;
			sphereTransform->position = glm::vec3(light->position);
			glm::mat4 matBall(1.0f);

			sphereProperties->materialDiffuse = oldDiffuse;
			sphereTransform->setUniformScale(0.5f);
			DrawObject(pDebugSphere, matBall, program);

			if (::g_bDrawDebugLightSpheres)
			{
				DrawDebugLightSpheres(pLightHelper, light, pDebugSphere, matBall, program, oldDiffuse, oldScale);
			}
		}//for ( sLight* light : g_Lights

		{
			// Draw the skybox first 
			Entity* pSkyBox = EntityManager::FindEntity("SkyBoxObject");
			Properties* skyBoxProperties = pSkyBox->GetComponent<Properties>();
			Transform* skyBoxTransform = pSkyBox->GetComponent<Transform>();

			skyBoxTransform->position = g_pCamera->eye;
			skyBoxProperties->bIsVisible = true;
			skyBoxProperties->bIsWireFrame = false;

			// Bind the cube map texture to the cube map in the shader
			GLuint cityTextureUNIT_ID = 30;			// Texture unit go from 0 to 79
			glActiveTexture(cityTextureUNIT_ID + GL_TEXTURE0);	// GL_TEXTURE0 = 33984

			int cubeMapTextureID = ::g_pTextureManager->getTextureIDFromName("CityCubeMap");

			// Cube map is now bound to texture unit 30
			//glBindTexture( GL_TEXTURE_2D, cubeMapTextureID );
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);

			//uniform samplerCube textureSkyBox;
			GLint skyBoxCubeMap_UniLoc = glGetUniformLocation(program, "textureSkyBox");
			glUniform1i(skyBoxCubeMap_UniLoc, cityTextureUNIT_ID);

			//uniform bool useSkyBoxTexture;
			GLint useSkyBoxTexture_UniLoc = glGetUniformLocation(program, "useSkyBoxTexture");
			glUniform1f(useSkyBoxTexture_UniLoc, (float)GL_TRUE);

			glm::mat4 matIdentity = glm::mat4(1.0f);
			DrawObject(pSkyBox, matIdentity, program);

			skyBoxProperties->bIsVisible = false;
			glUniform1f(useSkyBoxTexture_UniLoc, (float)GL_FALSE);
		}

		Update();

		//::g_pDebugRendererACTUAL->RenderDebugObjects(matView, matProjection, deltaTime);

		glfwSwapBuffers(window);

		glfwPollEvents();

		ProcessAsyncKeys(window);

		ProcessAsyncMouse(window);

	}//while (!glfwWindowShouldClose(window))

	return 0;
}

/**
* Update
* Code for the update phase
*/
void Update(void)
{
	{
		float dt = 0.3f;
		gBehaviourManager.Update(dt);

		for (System* s : gSystems)
		{
			s->Process(EntityManager::GetEntityList(), dt);
		}
	}

	Draw();
}

void Draw(void)
{
	for (Entity* e : EntityManager::GetEntityList())
	{
		glm::mat4x4 matModel = glm::mat4(1.0f);

		DrawObject(e, matModel, program);
	}
}

void DrawDebugLightSpheres(cLightHelper* pLightHelper, sLight* light, Entity* pDebugSphere, glm::mat4 matBall, GLuint program, glm::vec4 oldDiffuse, glm::vec3 oldScale)
{
	const float ACCURACY_OF_DISTANCE = 0.0001f;
	const float INFINITE_DISTANCE = 10000.0f;

	Properties* sphereProperties = pDebugSphere->GetComponent<Properties>();
	Transform* sphereTransform = pDebugSphere->GetComponent<Transform>();

	float distance90Percent =
		pLightHelper->calcApproxDistFromAtten(0.90f, ACCURACY_OF_DISTANCE,
			INFINITE_DISTANCE,
			light->atten.x,
			light->atten.y,
			light->atten.z);

	sphereTransform->setUniformScale(distance90Percent);			// 90% brightness
	sphereProperties->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
	DrawObject(pDebugSphere, matBall, program);

	float distance50Percent =
		pLightHelper->calcApproxDistFromAtten(0.50f, ACCURACY_OF_DISTANCE,
			INFINITE_DISTANCE,
			light->atten.x,
			light->atten.y,
			light->atten.z);
	sphereTransform->setUniformScale(distance50Percent);	// 50% brightness
	sphereProperties->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
	DrawObject(pDebugSphere, matBall, program);

	float distance25Percent =
		pLightHelper->calcApproxDistFromAtten(0.25f, ACCURACY_OF_DISTANCE,
			INFINITE_DISTANCE,
			light->atten.x,
			light->atten.y,
			light->atten.z);
	sphereTransform->setUniformScale(distance25Percent);	// 25% brightness
	sphereProperties->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
	DrawObject(pDebugSphere, matBall, program);

	float distance1Percent =
		pLightHelper->calcApproxDistFromAtten(0.01f, ACCURACY_OF_DISTANCE,
			INFINITE_DISTANCE,
			light->atten.x,
			light->atten.y,
			light->atten.z);
	sphereTransform->setUniformScale(distance1Percent);	// 1% brightness
	sphereProperties->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
	DrawObject(pDebugSphere, matBall, program);

	sphereProperties->materialDiffuse = oldDiffuse;
	sphereTransform->scale = oldScale;
	sphereProperties->bIsVisible = false;
}

