// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "Camera.h"
#include "Cubemap.h"
#include "Model.h"
#include "Mesh.h"
#include "shader.h"
#include "GameObject.h"
#include "ShadowMapFBO.h"
#include "Animation.h"
#include "Animator.h"
#include "PointLight.h"


//local1 = glm::mat4(1.0f);
//local1 = glm::rotate(sin(counter) * 20.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * local1;
//local1 = glm::translate(0.0f, 1.0f, 0.0f) * local1;
//Snowman.base.transform = local1 * glm::scale(1.5f, 1.5f, 1.5f);

//local2 = glm::mat4(1.0f);
//local2 = glm::rotate(sin(counter) * 20.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * local2;
//local2 = glm::translate(0.0f, 1.5f, 0.0f) * local2;
//Snowman.arm.transform = local1 * local2 * glm::scale(1.2f, 1.2f, 1.2f);;

//local21 = glm::mat4(1.0f);
//local21 = glm::rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(-sin(counter) * 40.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * local21;
//local21 = glm::translate(1.0f, 0.5f, 0.0f) * local21;
//Snowman.branch_right.transform = local1 * local2 * local21 * glm::scale(0.2f, 0.2f, 0.2f);

//local22 = glm::mat4(1.0f);
//local22 = glm::rotate(270.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(sin(counter) * 40.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * local22;
//local22 = glm::translate(-1.0f, 0.5f, 0.0f) * local22;
//Snowman.branch_left.transform = local1 * local2 * local22 * glm::scale(0.2f, 0.2f, 0.2f);


//local3 = glm::mat4(1.0f);
//local3 = glm::rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(270.0f, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(sin(counter) * 10.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * local3;
//local3 = glm::translate(0.0f, 1.5f, 0.0f) * local3;
//Snowman.head.transform = local1 * local2 * local3 * glm::scale(0.4f, 0.4f, 0.4f);

// laptop dir C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes

/*----------------------------------------------------------------------------
SHADERS
----------------------------------------------------------------------------*/

#define PVS_DEBUG			"../Graphics/Shaders/depth_debug.vert"
#define PFS_DEBUG			"../Graphics/Shaders/depth_debug.frag"
#define PVS_NAME			"../Graphics/Shaders/simpleVertexShader.vert"
#define PFS_NAME			"../Graphics/Shaders/simpleFragmentShader.frag"
#define PVS_TN				"../Graphics/Shaders/normal.vert"
#define PFS_TN				"../Graphics/Shaders/normal.frag"
#define PVS_SHADOW			"../Graphics/Shaders/shadow_mapping_vs.vert"
#define PFS_SHADOW			"../Graphics/Shaders/shadow_mapping_fs.frag"
#define PVS_SHADOW_DEPTH	"../Graphics/Shaders/shadow_mapping_depth.vert"
#define PFS_SHADOW_DEPTH	"../Graphics/Shaders/shadow_mapping_depth.frag"
#define PVS_SKYBOX			"../Graphics/Shaders/skybox.vert"
#define PFS_SKYBOX			"../Graphics/Shaders/skybox.frag"

/*----------------------------------------------------------------------------
MESHES
----------------------------------------------------------------------------*/
#define MESH_DEFAULT		"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/default.dae"
#define MESH_BATEMAN		"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/bateman.dae"
#define MESH_FLOOR			"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/snow_texture/snow_floor.obj"
#define MESH_PLACEHOLDER    "C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/penguin.dae"
#define MESH_LAMP			"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/lamp/lamp.obj"
#define MESH_IGLOO_1		"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/igloos/igloo_1.obj"
#define MESH_IGLOO_2		"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/igloos/igloo_2.obj"
#define MESH_IGLOO_3		"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/igloos/igloo_3.obj"
#define MESH_ICE_SURFACE_1	"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_surface_1/ice_surface_1.obj"
#define MESH_ICE_SURFACE_2	"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_surface_2/ice_surface_2.obj"
#define MESH_ICE_SURFACE_3	"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_surface_3/ice_surface_3.obj"
#define MESH_ICE_SURFACE_4	"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_surface_4/ice_surface_4.obj"
#define MESH_ICE_SURFACE_5	"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_surface_5/ice_surface_5.obj"
#define MESH_ICE_SURFACE_6	"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_surface_6/ice_surface_6.obj"
#define MESH_ICE_SURFACE_7	"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_surface_7/ice_surface_7.obj"
#define MESH_ICE_WALL_1		"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_wall_1/ice_wall_1.obj"
#define MESH_ICE_WALL_2		"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_wall_2/ice_wall_2.obj"
#define MESH_ICE_WALL_3		"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_wall_3/ice_wall_3.obj"
#define MESH_ICE_WALL_4		"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_wall_4/ice_wall_4.obj"
#define MESH_ICE_WALL_5		"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_wall_5/ice_wall_5.obj"
#define MESH_ICE_WALL_6		"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_wall_6/ice_wall_6.obj"
#define MESH_ICE_WALL_7		"C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/ice_wall_7/ice_wall_7.obj"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

using namespace std;

enum DebugState {
	SCALING, MOVING, ROTATING, SWITCHING
};

enum ObjectState{
	PENGUINS, OBJECTS, LIGHTS
};

DebugState debug_state = MOVING;
ObjectState object_state = PENGUINS;

Camera player_camera = Camera();

ShadowMapFBO shadow_fbo;
Cubemap skybox;
Shader lightShader, shadowShader, debugShader, skyboxShader, normalShader;

Model penguin, game_map, iceberg;

Animation penguin_walk;
Animator animator;

GameObject* selected;
GameObject lightCursor = GameObject();

glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -60.0f, 60.f);
glm::mat4 lightSpaceMatrix;
glm::vec3 lightPosition = glm::vec3(-1.0f,0.0f,0.0f);
glm::vec3 origin = glm::vec3(0.0f);
glm::vec3 centre = glm::vec3(-17.0f,0.0f,9.0f);

bool isDebug = false;
bool isSunRotating = false;
bool isDepthMap = false;
bool isDay = true;

GameObject character, bateman;

std::vector<GameObject> Objects, Penguins;
std::vector<Model> Models;
std::vector<PointLight> Lights;

bool firstMouse = true;
bool moveForward = false;
bool moveBackward = false;
bool printPos = false;

int width = 1920;
int height = 1080;
int shadowrez = 16000;
int numPointLights = 0;
int LightIndex = 0;
int ObjectIndex = 0;
int ModelIndex = 1;
unsigned int quadVAO = 0;
unsigned int quadVBO;

float offset = 2.0f;
float speed=5;
float maxCount = 10;
float counter = 0;
float direction = 0.f;
float delta = 0.0f;
float delta_X = width / 2.0f;
float delta_Y = height / 2.0f;

GLfloat rotate_y = 0.0f;

glm::mat4 local1, local2, local3, local21, local22;

struct {
	GameObject base, arm, head, branch_left, branch_right;
}Snowman;

glm::vec3 cubePositions[] = {
	   glm::vec3(0.0f,  2.0f,  0.0f),
	   glm::vec3(2.0f,  5.0f, -15.0f),
	   glm::vec3(-1.5f, -2.2f, -2.5f),
	   glm::vec3(-3.8f, -2.0f, -12.3f),
	   glm::vec3(2.4f, -0.4f, -3.5f),
	   glm::vec3(-1.7f,  3.0f, -7.5f),
	   glm::vec3(1.3f, -2.0f, -2.5f),
	   glm::vec3(1.5f,  2.0f, -2.5f),
	   glm::vec3(1.5f,  0.2f, -1.5f),
	   glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] = {
		glm::vec3(-33.1784f, 0.239569f, 6.61508f),
		glm::vec3(-27.4434f, 0.205384f, 21.9051f),
		glm::vec3(-31.4852f, 6.04804f, 14.3013f),
		glm::vec3(1.76556f, 0.39406f, 2.10703f)
};

void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void ShadowPass() {
	shadow_fbo.BindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);
	
	shadowShader.Use();
	shadowShader.SetMatrix4("lightSpaceMatrix", lightSpaceMatrix, false);
	for (int i = 0; i < Objects.size(); i++) {
		Objects[i].Draw(&shadowShader);
	}

	for (int i = 0; i < Penguins.size(); i++) {
		Penguins[i].Draw(&shadowShader);
	}
}

void SkyPass(float ratio) {
	glm::mat4 view = glm::mat4(glm::mat3(player_camera.GetViewMatrix()));

	skyboxShader.Use();
	skyboxShader.SetFloat("ratio", ratio, false);
	skyboxShader.SetMatrix4("view", view, false);
	skyboxShader.SetMatrix4("proj", player_camera.GetProjection(), false);
	skybox.Draw();
}

void LightPass(float ratio) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float fraction = 1 - ratio;

	shadow_fbo.BindForReading(GL_TEXTURE0);

	lightShader.Use();
	lightShader.SetInteger("shadowMap", 0, false);
	lightShader.SetInteger("texture_diffuse1", 1, false);
	lightShader.SetInteger("texture_normal1", 2, false);
	lightShader.SetMatrix4("proj", player_camera.GetProjection(), false);
	lightShader.SetMatrix4("view", player_camera.GetViewMatrix(), false);
	lightShader.SetVector3f("dirLight.direction", lightPosition, false);
	lightShader.SetVector3f("dirLight.ambient", 0.9f * fraction, 0.9f * fraction, 0.9f * fraction, false);
	lightShader.SetVector3f("viewPos", player_camera.Position, false);
	lightShader.SetMatrix4("lightSpaceMatrix", lightSpaceMatrix, false);

	for (int i = 0; i < Objects.size(); i++) {
		Objects[i].Draw(&lightShader);
	}

	for (int i = 0; i < Penguins.size(); i++) {
		Penguins[i].Draw(&lightShader);
	}
}

void Debug() {
	debugShader.SetInteger("depthMap", 0, true);
	debugShader.SetFloat("near_plane", -100.0f, true);
	debugShader.SetFloat("far_plane", 100.0f, true);

	shadow_fbo.BindForReading(GL_TEXTURE0);

	renderQuad();
}

void display() {
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//std::cout << "CAMERA: " << player_camera.Position.x << ", " << player_camera.Position.y << ", " << player_camera.Position.z << ", YAW: " << player_camera.Yaw << " PITCH: " << player_camera.Pitch << "\n";
	
	if (!isSunRotating)
		counter += delta* 0.1;

	if (counter > maxCount) {
		counter = 0;
		isDay = !isDay;
		if (isDay) {
			lightShader.SetVector3f("dirLight.diffuse", 0.8f, 0.8f, 0.8f, true);
			lightShader.SetVector3f("dirLight.specular", 0.2f, 0.2f, 0.2f, true);
		}
		else {
			lightShader.SetVector3f("dirLight.diffuse", 0.005f, 0.005f, 0.005f, true);
			lightShader.SetVector3f("dirLight.specular", 0.1f, 0.1f, 0.1f, true);
		}
			
	}
	
	float ratio;
	float fraction = counter / maxCount;
	float radAngle = fraction * 3.1415;

    if (isDay)
		ratio = abs(cos(radAngle));
	else
		ratio = 1;

	lightPosition = glm::vec3(cos(radAngle), sin(radAngle), 0.0f);
	glm::mat4 lightView = glm::lookAt(lightPosition, origin, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;

	glCullFace(GL_FRONT);
	ShadowPass();
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	LightPass(ratio);
	if(isDepthMap)
		Debug();

	SkyPass(ratio);

	//cout << " POS: " << Objects[1].position.x << " , " << Objects[1].position.y << " , " << Objects[1].position.z << "\n";

	glutSwapBuffers();
}

void updateAnimation() {
	for (int i = 0; i < Penguins.size(); i++) {
		if (Penguins[i].isAnimatied)
			Penguins[i].animator.UpdateAnimation(delta * glm::length(Penguins[i].velocity * 5.0f));
	}
}

void updateDirection() {
	for (int i = 1; i < Penguins.size(); i++) {
		float dist = Penguins[i].distanceTo(Penguins[0].position);
		Penguins[i].aimToPoint(Penguins[0].position, min(2 * offset, dist) - offset);
		for (int j = 1; j < Penguins.size(); j++) {
			dist = Penguins[i].distanceTo(Penguins[j].position);
			if (dist < offset)
				Penguins[i].aimToPoint(Penguins[j].position, dist - offset);
		}
	}
}
void updateStep() {
	for (int i = 0; i < Penguins.size(); i++) {
		//if (printPos)
		//	cout << "Object " << i << " : " << Penguins[i].position.x << " , " << Penguins[i].position.y << " , " << Penguins[i].position.z << "\n";
		Penguins[i].Step(delta);
	}
	selected->Step(delta);
}

void printObject(GameObject object) {
	cout << "Model:    " << object.model->directory << "\n";
	cout << "Position: " << object.position.x<<", "<< object.position.y <<", "<< object.position.z << "\n";
	cout << "Rotation: " << object.RotX << ", " << object.RotY << ", " << object.RotZ << "\n";
	cout << "Scale:    " << object.ScaleX << ", " << object.ScaleY << ", " << object.ScaleZ << "\n";
}

void printLight(PointLight light) {
	cout << "Position: " << light.position.x << ", " << light.position.y << ", " << light.position.z << "\n";
	cout << "Strength:    " << light.strength << "\n";
}
void updateScene() {
	
	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

	if (moveForward) {
		selected->updateVelocity(selected->direction * speed);
		moveForward = false;
	}
	if (moveBackward) {
		selected->updateVelocity(selected->direction * -speed);
		moveBackward = false;
	}

	if (printPos) {
		cout << "\nOBJECTS\n\n";
		for (int i = 0; i < Objects.size(); i++) {
			cout << "Object " << i <<"\n";
			printObject(Objects[i]);
			cout << "\n";
		}
		cout << "\nLIGHTS\n\n";
		for (int i = 0; i < Lights.size(); i++) {
			cout << "Light " << i << "\n";
			printLight(Lights[i]);
			cout << "\n";
		}
		cout << "\n\n";
		printPos = !printPos;
	}

	updateDirection();
	updateAnimation();
	updateStep();

	printPos = false;

	// Draw the next frame
	glutPostRedisplay();
	display();
}

void newLight(glm::vec3 pos, float str) {
	Lights.push_back(PointLight(pos));
	Lights[numPointLights].strength = str;
	Lights[numPointLights].index = numPointLights;
	Lights[numPointLights].Update(&lightShader);
	numPointLights++;	
}

void printInstructions() {
	cout << "\nINSTRUCTIONS" << "\n";
	cout << "You can print this page again by pressing H\n";
	cout << "\n";
	cout << "To control the camera click on the screen and use W S A D to move around" << "\n";
	cout << "\n";
	cout << "You can switch between moving the penguin group around, manipulating objects and manipulating lights in the scene using P O and L respectively." << "\n";
	cout << "\n";
	cout << "CAMERA" << "\n";
	cout << "	Move: 					WSAD" << "\n";
	cout << "	Rotate: 				hold mouse button and move the cursor" << "\n";
	cout << "	Reset:					R" << "\n";
	cout << "\n";
	cout << "MODE SWITCHING" << "\n";
	cout << "	Penguins: 				P" << "\n";
	cout << "	Objects: 				O" << "\n";
	cout << "	Lights: 				L" << "\n";
	cout << "\n";
	cout << "MISCELLANOUS" << "\n";
	cout << "	Stop the sun 0.0			Z" << "\n";
	cout << "	See Depth Map				X" << "\n";
	cout << "	Print Object/Light data 		C" << "\n";
	cout << "	Scaling,moving,rotating strength	, to lower . to increase" << "\n";
	cout << "" << "\n";
	cout << "PENGUINS" << "\n";
	cout << "	Move: 					arrow keys" << "\n";
	cout << "	New Penguin: 				N" << "\n";
	cout << "" << "\n";
	cout << "OBJECTS" << "\n";
	cout << "	New Model				N" << "\n";
	cout << "	MODE SWITCHING" << "\n";
	cout << "		MOVE: 				1" << "\n";
	cout << "		ROTATE: 			2" << "\n";
	cout << "		SCALE: 				3" << "\n";
	cout << "		SWITCH MODELS/OBJECTS:	 	4" << "\n";
	cout << "" << "\n";
	cout << "	MOVE: 					arrow keys for left,right,forward,back" << "\n";
	cout << "						pageup pagedown for up down" << "\n";
	cout << "	ROTATE: 				as above" << "\n";
	cout << "	SCALE: 					as above" << "\n";
	cout << "	SWITCH: 				up down for switching models" << "\n";
	cout << "						left right for switching objects" << "\n";
	cout << "LIGHTS" << "\n";
	cout << "	New Light				N" << "\n";
	cout << "	MODE SWITCHING" << "\n";
	cout << "		MOVE: 				1" << "\n";
	cout << "		MODIFY LIGHT STRENGTH:	 	3" << "\n";
	cout << "		SWITCH LIGHTS: 			4" << "\n";
	cout << "	MOVE:					arrow keys for left,right,forward,back" << "\n";
	cout << "						pageup pagedown for up down" << "\n";
	cout << "	MODIFY LIGHT STRENGTH:			up down to scale the brightness" << "\n";
	cout << "	SWITCH:					left right to switch lights" << "\n";
}

// compiling shaders, load models, lights and create a scene
void init()
{
	skyboxShader = Shader(PVS_SKYBOX, PFS_SKYBOX);
	skyboxShader.SetInteger("day", 0, true);
	skyboxShader.SetInteger("night", 1, true);

	lightShader = Shader(PVS_TN, PFS_TN);
	lightShader.SetVector3f("dirLight.ambient", 0.1f, 0.1f, 0.1f, true);
	lightShader.SetVector3f("dirLight.diffuse", 0.8f, 0.8f, 0.8f, true);
	lightShader.SetVector3f("dirLight.specular", 0.5f, 0.5f, 0.5f, true);

	shadowShader = Shader(PVS_SHADOW_DEPTH, PFS_SHADOW_DEPTH);
	debugShader = Shader(PVS_DEBUG, PFS_DEBUG);

	shadow_fbo.Init(shadowrez, shadowrez, false);

	player_camera.SetProjection(PERSP);

	skybox = Cubemap();
	skybox.loadCubemap(vector<std::string>
	{
		"../Graphics/Textures/skybox/clouds_right.bmp",
		"../Graphics/Textures/skybox/clouds_left.bmp",
		"../Graphics/Textures/skybox/clouds_top.bmp",
		"../Graphics/Textures/skybox/clouds_bottom.bmp",
		"../Graphics/Textures/skybox/clouds_front.bmp",
		"../Graphics/Textures/skybox/clouds_back.bmp"
	});
	skybox.loadCubemap(vector<std::string>
	{
		"../Graphics/Textures/skybox/night_right.png",
		"../Graphics/Textures/skybox/night_left.png",
		"../Graphics/Textures/skybox/night_top.png",
		"../Graphics/Textures/skybox/night_bottom.png",
		"../Graphics/Textures/skybox/night_front.png",
		"../Graphics/Textures/skybox/night_back.png"
	});

	skybox.setupCubemap();
	
	penguin = Model(MESH_PLACEHOLDER);
	Models.push_back(penguin);
	Models.push_back(Model(MESH_FLOOR));
	Models.push_back(Model(MESH_LAMP));
	Models.push_back(Model(MESH_IGLOO_1));
	Models.push_back(Model(MESH_IGLOO_2));
	Models.push_back(Model(MESH_IGLOO_3));
	Models.push_back(Model(MESH_ICE_WALL_2));
	Models.push_back(Model(MESH_ICE_WALL_3));

	
	penguin_walk = Animation(MESH_PLACEHOLDER, &penguin);

	Lights.push_back(PointLight(glm::vec3(0.0f, 0.1f, 0.0f)));
	Lights[0].index = 0;
	Lights[0].Update(&lightShader);
	numPointLights++;

	GameObject new_object;

	new_object = GameObject(&Models[1]);
	new_object.hasNormalMap = true;
	new_object.Move(0.0f, 0.0f, 0.0f);
	new_object.Rotate(0.0f, 0.0f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);
	Objects.push_back(new_object);

	new_object = GameObject(&Models[4]);
	new_object.hasNormalMap = true;
	new_object.Move(-30.0f, 0.0f, -15.0f);
	new_object.Rotate(0.0f, -50.0f, 0.0f);
	new_object.Scale(6.0f, 6.0f, 6.0f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[3]);
	new_object.hasNormalMap = true;
	new_object.Move(-57.5f, 0.0f, -5.0f);
	new_object.Rotate(0.0f, -70.0f, 0.0f);
	new_object.Scale(3.5f, 3.5f, 3.5f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[4]);
	new_object.hasNormalMap = true;
	new_object.Move(-12.5f, 0.0f, -25.0f);
	new_object.Rotate(0.0f, 0.0f, 0.0f);
	new_object.Scale(2.25f, 2.25f, 2.25f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[4]);
	new_object.hasNormalMap = true;
	new_object.Move(0.0f, 0.0f, -10.0f);
	new_object.Rotate(0.0f, -85.0f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[4]);
	new_object.hasNormalMap = true;
	new_object.Move(-15.0f, 0.0f, 5.0f);
	new_object.Rotate(0.0f, 0.0f, 0.0f);
	new_object.Scale(1.625f, 1.625f, 1.625f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[4]);
	new_object.hasNormalMap = true;
	new_object.Move(21.25f, 0.0f, 12.5f);
	new_object.Rotate(0.0f, 165.0f, 0.0f);
	new_object.Scale(1.3125f, 1.3125f, 1.3125f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[4]);
	new_object.hasNormalMap = true;
	new_object.Move(0.0f, 0.0f, -35.0f);
	new_object.Rotate(0.0f, -32.5f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[4]);
	new_object.hasNormalMap = true;
	new_object.Move(-37.5f, 0.0f, 17.5f);
	new_object.Rotate(1.25f, -13.75f, 0.0f);
	new_object.Scale(2.25f, 2.25f, 2.25f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[3]);
	new_object.hasNormalMap = true;
	new_object.Move(-12.5f, 0.0f, 28.75f);
	new_object.Rotate(0.0f, 113.75f, 0.0f);
	new_object.Scale(1.9375f, 1.625f, 1.9375f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[5]);
	new_object.hasNormalMap = true;
	new_object.Move(21.5625f, 0.0f, -25.9375f);
	new_object.Rotate(0.0f, 194.062f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[3]);
	new_object.hasNormalMap = true;
	new_object.Move(-32.5f, 0.0f, 11.25f);
	new_object.Rotate(0.0f, -50.0f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[3]);
	new_object.hasNormalMap = true;
	new_object.Move(6.25f, 0.0f, -47.5f);
	new_object.Rotate(0.0f, -127.5f, 0.0f);
	new_object.Scale(2.25f, 2.25f, 2.25f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[3]);
	new_object.hasNormalMap = true;
	new_object.Move(18.75f, 0.0f, 32.5f);
	new_object.Rotate(0.0f, -263.75f, 0.0f);
	new_object.Scale(2.25f, 2.25f, 2.25f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[3]);
	new_object.hasNormalMap = true;
	new_object.Move(-5.0f, 0.0f, 17.5f);
	new_object.Rotate(0.0f, 142.5f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[3]);
	new_object.hasNormalMap = true;
	new_object.Move(37.5f, 0.0f, -10.0f);
	new_object.Rotate(0.0f, -221.25f, 0.0f);
	new_object.Scale(1.625f, 2.25f, 1.625f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[3]);
	new_object.hasNormalMap = true;
	new_object.Move(-12.5f, 0.0f, -12.5f);
	new_object.Rotate(0.0f, -90.0f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[5]);
	new_object.hasNormalMap = true;
	new_object.Move(-5.0f, 0.0f, -20.0f);
	new_object.Rotate(0.0f, 60.0f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[5]);
	new_object.hasNormalMap = true;
	new_object.Move(-35.0f, 0.0f, 27.5f);
	new_object.Rotate(0.0f, 70.0f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[5]);
	new_object.hasNormalMap = true;
	new_object.Move(14.375f, 0.0f, 33.75f);
	new_object.Rotate(0.0f, 145.0f, 0.0f);
	new_object.Scale(1.625f, 1.625f, 1.625f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[6]);
	new_object.hasNormalMap = true;
	new_object.Move(-2.5, 0.0f, -45.4688f);
	new_object.Rotate(0.0f, 2.5f, 0.0f);
	new_object.Scale(7.875f, 6.625f, 2.25f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[6]);
	new_object.hasNormalMap = true;
	new_object.Move(37.5f, 0.0f, -27.5f);
	new_object.Rotate(0.0f, -47.5f, 0.0f);
	new_object.Scale(8.5f, 6.0f, 8.5f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[7]);
	new_object.hasNormalMap = true;
	new_object.Move(-37.5f, 0.0f, -35.0f);
	new_object.Rotate(0.0f, 31.875f, 0.0f);
	new_object.Scale(6.0f, 5.375f, 7.25f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[7]);
	new_object.hasNormalMap = true;
	new_object.Move(-52.5f, 0.0f, -10.0f);
	new_object.Rotate(0.0f, 82.5f, 0.0f);
	new_object.Scale(6.0f, 6.0f, 8.5f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[6]);
	new_object.hasNormalMap = true;
	new_object.Move(-45.0f, 0.0f, 27.5f);
	new_object.Rotate(0.0f, 122.5f, 0.0f);
	new_object.Scale(8.5f, 8.5f, 8.5f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[6]);
	new_object.hasNormalMap = true;
	new_object.Move(32.5f, 0.0f, 32.5f);
	new_object.Rotate(0.0f, -147.5f, 0.0f);
	new_object.Scale(6.0f, 8.5f, 8.5f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[6]);
	new_object.hasNormalMap = true;
	new_object.Move(-5.0f, 0.0f, 45.0f);
	new_object.Rotate(0.0f, -180.0f, 0.0f);
	new_object.Scale(8.5f, 8.5f, 8.5f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[7]);
	new_object.hasNormalMap = true;
	new_object.Move(47.5f, 0.0f, 7.5f);
	new_object.Rotate(0.0f, -101.875f, 0.0f);
	new_object.Scale(7.875f, 4.75f, 7.875f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(-10.0f, 0.0f, -12.5f);
	new_object.Rotate(0.0f, 47.5f, 0.0f);
	new_object.Scale(3.5f, 3.5f, 3.5f);
	newLight(glm::vec3(new_object.position.x, 5, new_object.position.z), 8.5);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(-18.75f, 0.0f, -6.25f);
	new_object.Rotate(0.0f, 32.5f, 0.0f);
	new_object.Scale(3.5f, 3.5f, 3.5f);
	newLight(glm::vec3(new_object.position.x, 5, new_object.position.z), 8.5);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(-11.875f, 0.0f, 5.0f);
	new_object.Rotate(0.0f, 62.5f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);
	newLight(glm::vec3(new_object.position.x, 1.25, new_object.position.z), 2.25);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(-2.5f, 0.0f, -10.0f);
	new_object.Rotate(0.0f, -15.0f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);
	newLight(glm::vec3(new_object.position.x, 1.25, new_object.position.z), 2.25);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(-25.0f, 0.0f, 7.5f);
	new_object.Rotate(2.5f, 45.0f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);
	newLight(glm::vec3(new_object.position.x, 1.25, new_object.position.z), 2.25);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(2.5f, 0.0f, -15.0f);
	new_object.Rotate(0.0f, -70.0f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);
	newLight(glm::vec3(new_object.position.x, 1.25, new_object.position.z), 2.25);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(-32.5f, 0.0f, 22.5f);
	new_object.Rotate(0.0f, -77.5f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);
	newLight(glm::vec3(new_object.position.x, 1.25, new_object.position.z), 2.25);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(-22.5f, 0.0f, 28.75f);
	new_object.Rotate(0.0f, 5.0f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);
	newLight(glm::vec3(new_object.position.x, 1.25, new_object.position.z), 2.25);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(1.25f, 0.0f, -33.75f);
	new_object.Rotate(0.0f, 20.0f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);
	newLight(glm::vec3(new_object.position.x, 1.25, new_object.position.z), 2.25);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(10.0f, 0.0f, -36.25f);
	new_object.Rotate(1.25f, 0.0f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);
	newLight(glm::vec3(new_object.position.x, 1.25, new_object.position.z), 2.25);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(18.75f, 0.0f, -28.75f);
	new_object.Rotate(0.0f, -42.5f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);
	newLight(glm::vec3(new_object.position.x, 1.25, new_object.position.z), 2.25);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(21.25f, 0.0f, 7.5f);
	new_object.Rotate(1.25f, -103.75f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);
	newLight(glm::vec3(new_object.position.x, 1.25, new_object.position.z), 2.25);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(27.5f, 0.0f, -8.75f);
	new_object.Rotate(0.0f, -71.25f, 0.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);
	newLight(glm::vec3(new_object.position.x, 1.25, new_object.position.z), 2.25);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(11.25f, 0.0f, 27.5f);
	new_object.Rotate(0.0f, 23.75f, 0.0f);
	new_object.Scale(2.25f, 2.25f, 2.25f);
	newLight(glm::vec3(new_object.position.x, 3.75, new_object.position.z), 4.75);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(-6.25f, 0.0f, -31.25f);
	new_object.Rotate(0.0f, 71.25f, 0.0f);
	new_object.Scale(2.25f, 2.25f, 2.25f);
	newLight(glm::vec3(new_object.position.x, 3.75, new_object.position.z), 4.75);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[2]);
	new_object.hasNormalMap = true;
	new_object.Move(-36.25f, 0.0f, -2.5f);
	new_object.Rotate(0.0f, 0.0f, 0.0f);
	new_object.Scale(2.25f, 2.25f, 2.25f);
	newLight(glm::vec3(new_object.position.x, 3.75, new_object.position.z), 4.75);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[0]);
	new_object.hasNormalMap = true;
	new_object.Move(-15.0f, 0.0f, -5.0f);
	new_object.Rotate(-95.0f, 0.0f, 20.0f);
	new_object.Scale(1.0f, 1.0f, 1.0f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[0]);
	new_object.hasNormalMap = true;
	new_object.Move(-8.75f, 0.0f, -10.0f);
	new_object.Rotate(-96.25f, 0.0f, 56.25f);
	new_object.Scale(1.0f, 1.0f, 1.0f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[0]);
	new_object.hasNormalMap = true;
	new_object.Move(0.15625f, 0.0f, -6.95312f);
	new_object.Rotate(85.0f, 0.0f, -196.875f);
	new_object.Scale(0.25f, 0.25f, 0.25f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[0]);
	new_object.hasNormalMap = true;
	new_object.Move(-10.3125f, 0.0f, 6.25f);
	new_object.Rotate(-78.3594f, -1.25f, -283.125f);
	new_object.Scale(0.25f, 0.25f, 0.25f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[0]);
	new_object.hasNormalMap = true;
	new_object.Move(-29.6875, 0.0f, 12.5f);
	new_object.Rotate(-101.25, 5, 52.5f);
	new_object.Scale(0.25f, 0.25f, 0.25f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[0]);
	new_object.hasNormalMap = true;
	new_object.Move(-14.0625, 0.0f, 24.375f);
	new_object.Rotate(-97.5, 0.0f, 227.5f);
	new_object.Scale(0.25f, 0.25f, 0.25f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[0]);
	new_object.hasNormalMap = true;
	new_object.Move(9.375, 0.0f, -31.25f);
	new_object.Rotate(-95, 0.0f, 0.0f);
	new_object.Scale(0.25f, 0.25f, 0.25f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[0]);
	new_object.hasNormalMap = true;
	new_object.Move(-2.5, 0.0f, -23.75f);
	new_object.Rotate(-91.25, 0.0f, 102.5f);
	new_object.Scale(0.25f, 0.25f, 0.25f);

	Objects.push_back(new_object);

	new_object = GameObject(&Models[0]);
	new_object.hasNormalMap = true;
	new_object.Move(17.5, 0.0f, 20.0f);
	new_object.Rotate(-80.0f, 3.75, -124.688);
	new_object.Scale(0.25f, 0.25f, 0.25f);

	Objects.push_back(new_object);

	for (int i = 0; i < 10; i++) {
		GameObject pengo = GameObject(&penguin);
		pengo.Move(i, 0.0f, i);
		pengo.Scale(0.025f, 0.025f, 0.025f);
		pengo.setAnimation(&penguin_walk);
		pengo.isAnimatied = true;
		pengo.hasShadow = false;
		Penguins.push_back(pengo);
	}

	LightIndex = numPointLights - 1;
	lightShader.SetInteger("numPointLights", numPointLights, true);
	selected = &Penguins[0];
	printInstructions();
}

void mouseMove(int x, int y) {
	if (firstMouse) {
		delta_X = x;
		delta_Y = y;
		firstMouse = false;
	}

	float xoffset = x - delta_X;
	float yoffset = delta_Y - y; // reversed since y-coordinates go from bottom to top

	delta_X = x;
	delta_Y = y;

	player_camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouseButton(int button, int state, int x, int y) {
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {
		// when the button is released
		if (state == GLUT_DOWN) {
			delta_X = x;
			delta_Y = y;
		}
	}
}

void arrowKeyes(int key, int x, int y) {

	if (object_state == OBJECTS) {
		switch (debug_state) {
		case SCALING:
			switch (key)
			{
			case GLUT_KEY_UP:
				selected->Scale(selected->ScaleX, selected->ScaleY, selected->ScaleZ + speed);
				break;
			case GLUT_KEY_DOWN:
				selected->Scale(selected->ScaleX, selected->ScaleY, selected->ScaleZ - speed);
				break;
			case GLUT_KEY_LEFT:
				selected->Scale(selected->ScaleX + speed, selected->ScaleY, selected->ScaleZ);
				break;
			case GLUT_KEY_RIGHT:
				selected->Scale(selected->ScaleX -speed, selected->ScaleY, selected->ScaleZ);
				break;
			case GLUT_KEY_PAGE_UP:
				selected->Scale(selected->ScaleX, selected->ScaleY + speed, selected->ScaleZ);
				break;
			case GLUT_KEY_PAGE_DOWN:
				selected->Scale(selected->ScaleX, selected->ScaleY - speed, selected->ScaleZ);
				break;
			}
			break;

		case ROTATING:
			switch (key)
			{
			case GLUT_KEY_LEFT:
				selected->Rotate(0.0f, speed, 0.0f);
				break;
			case GLUT_KEY_RIGHT:
				selected->Rotate(0.0f, -speed, 0.0f);
				break;
			case GLUT_KEY_UP:
				selected->Rotate(-speed, 0.0f, 0.0f);
				break;
			case GLUT_KEY_DOWN:
				selected->Rotate(speed, 0.0f, 0.0f);
				break;
			case GLUT_KEY_PAGE_UP:
				selected->Rotate(0.0f, 0.0f, speed);
				break;
			case GLUT_KEY_PAGE_DOWN:
				selected->Rotate(0.0f, 0.0f, -speed);
				break;
			}
			break;

		case SWITCHING:
			switch (key)
			{
			case GLUT_KEY_UP:
				if (ModelIndex > 0)
					ModelIndex--;
				selected->SwitchModel(&Models[ModelIndex]);
				break;
			case GLUT_KEY_DOWN:
				if (ModelIndex < Models.size() - 1)
					ModelIndex++;
				selected->SwitchModel(&Models[ModelIndex]);
				break;
			case GLUT_KEY_LEFT:
				if (ObjectIndex > 0)
					ObjectIndex--;
				selected = &Objects[ObjectIndex];
				break;
			case GLUT_KEY_RIGHT:
				if (ObjectIndex < Objects.size() - 1)
					ObjectIndex++;
				selected = &Objects[ObjectIndex];
				break;
			}
			break;

		case MOVING:
			switch (key)
			{
			case GLUT_KEY_PAGE_UP:
				selected->Move(0.0f, speed, 0.0f);
				break;
			case GLUT_KEY_PAGE_DOWN:
				selected->Move(0.0f, -speed, 0.0f);
				break;
			case GLUT_KEY_LEFT:
				selected->Move(-speed, 0.0f, 0.0f);
				break;
			case GLUT_KEY_RIGHT:
				selected->Move(speed, 0.0f, 0.0f);
				break;
			case GLUT_KEY_UP:
				selected->Move(0.0f, 0.0f, speed);
				break;
			case GLUT_KEY_DOWN:
				selected->Move(0.0f, 0.0f, -speed);
				break;
			}
			break;

		default:
			break;
		}
		return;
	}

	if (object_state == LIGHTS) {
		switch (debug_state) {
		case SCALING:
			switch (key)
			{
			case GLUT_KEY_UP:
				Lights[LightIndex].strength = Lights[LightIndex].strength + speed;
				Lights[LightIndex].Update(&lightShader);
				break;
			case GLUT_KEY_DOWN:
				Lights[LightIndex].strength = Lights[LightIndex].strength - speed;
				Lights[LightIndex].Update(&lightShader);
				break;
			}
			break;

		case SWITCHING:
			switch (key)
			{
			case GLUT_KEY_LEFT:
				if (LightIndex > 0)
					LightIndex--;
				selected->position = Lights[LightIndex].position;
				Lights[LightIndex].Update(&lightShader);
				break;
			case GLUT_KEY_RIGHT:
				if (LightIndex < Lights.size() - 1)
					LightIndex++;
				selected->position = Lights[LightIndex].position;
				Lights[LightIndex].Update(&lightShader);
				break;
			}
			break;

		case MOVING:
			switch (key)
			{
			case GLUT_KEY_PAGE_UP:
				selected->Move(0.0f, speed, 0.0f);
				Lights[LightIndex].position = selected->position;
				Lights[LightIndex].Update(&lightShader);
				break;
			case GLUT_KEY_PAGE_DOWN:
				selected->Move(0.0f, -speed, 0.0f);
				Lights[LightIndex].position = selected->position;
				Lights[LightIndex].Update(&lightShader);
				break;
			case GLUT_KEY_LEFT:
				selected->Move(-speed, 0.0f, 0.0f);
				Lights[LightIndex].position = selected->position;
				Lights[LightIndex].Update(&lightShader);
				break;
			case GLUT_KEY_RIGHT:
				selected->Move(speed, 0.0f, 0.0f);
				Lights[LightIndex].position = selected->position;
				Lights[LightIndex].Update(&lightShader);
				break;
			case GLUT_KEY_UP:
				selected->Move(0.0f, 0.0f, speed);
				Lights[LightIndex].position = selected->position;
				Lights[LightIndex].Update(&lightShader);
				break;
			case GLUT_KEY_DOWN:
				selected->Move(0.0f, 0.0f, -speed);
				Lights[LightIndex].position = selected->position;
				Lights[LightIndex].Update(&lightShader);
				break;
			}
			break;

			default:
				break;
		}
		return;
	}

	switch (key)
	{
	case GLUT_KEY_UP:
		moveForward = true;
		break;
	case GLUT_KEY_DOWN:
		moveBackward = true;
		break;
	case GLUT_KEY_LEFT:
		direction += 11.25f * speed;
		if (direction > 360)
			direction -= 360;
		selected->setDirection(direction, DEG);
		break;
	case GLUT_KEY_RIGHT:
		direction -= 11.25f * speed;
		if (direction < -360)
			direction += 360;
		selected->setDirection(direction, DEG);
		break;
	}
}

void newObject(Model* model) {
	GameObject new_object = GameObject(model);
	Objects.push_back(new_object);
}

void keypress(unsigned char key, int x, int y) {

	int mod = glutGetModifiers();

	player_camera.CameraKeyboard(key, mod, delta);

	if (key == 'p') {
		cout << "\nPENGUIN MODE\n";
		object_state = PENGUINS;
		selected = &Penguins[0];
	}
	if (key == 'o') {
		cout << "\nOBJECT MODE\n";
		object_state = OBJECTS;
		selected = &Objects[ObjectIndex];
	}
	if (key == 'l') {
		cout << "\nLIGHT MODE\n";
		object_state = LIGHTS;
		selected = &lightCursor;
	}
	if (key == 'r')
		player_camera.Reset();
	if (key == 'z')
		isSunRotating = !isSunRotating;
	if (key == 'x')
		isDepthMap = !isDepthMap;
	if (key == 'c') {
		printPos = true;
	}
	if (key == 'h') {
		printInstructions();
	}
	if (key == 'v') {
		cout << "Object Index " << ObjectIndex << "\n";
		cout << "Model Index " << ModelIndex << "\n\n";
	}
	if (key == '.') {
		speed *= 2;
		cout << "speed: " << speed << "\n";
	}
	if (key == ',') {
		speed /= 2;
		cout << "speed: " << speed << "\n";
	}
	if (key == 'n') {
		if (object_state == PENGUINS) {
			GameObject pengo = GameObject(&penguin);
			pengo.Scale(0.025f, 0.025f, 0.025f);
			pengo.setAnimation(&penguin_walk);
			pengo.isAnimatied = true;
			pengo.hasShadow = false;
			Penguins.push_back(pengo);
		}
		if (object_state == OBJECTS) {
			newObject(&Models[ModelIndex]);
			ObjectIndex = Objects.size() - 1;
			Objects[ObjectIndex].hasNormalMap = true;
			selected = &Objects[ObjectIndex];
		}
		if (object_state == LIGHTS) {
			Lights.push_back(PointLight(selected->position));
			Lights[numPointLights].index = numPointLights;
			Lights[numPointLights].Update(&lightShader);
			numPointLights++;
			lightShader.SetInteger("numPointLights", numPointLights, true);
		}
	}
	if (object_state != PENGUINS) {

		if (key == '1')
			debug_state = MOVING;
		if (key == '2')
			debug_state = ROTATING;
		if (key == '3')
			debug_state = SCALING;
		if (key == '4')
			debug_state = SWITCHING;
		

	}
}

int main(int argc, char** argv) {
	// Set up the window
	glutInit(&argc, argv);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("Hello Triangle");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	glutMotionFunc(mouseMove);
	glutSpecialFunc(arrowKeyes);
	glutMouseFunc(mouseButton);

	srand(static_cast <unsigned> (time(0)));

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
