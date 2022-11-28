// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
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
#include "Model.h"
#include "Mesh.h"
#include "shader.h"
#include "GameObject.h"
#include "ShadowMapFBO.h"


// laptop dir C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes

/*----------------------------------------------------------------------------
SHADERS
----------------------------------------------------------------------------*/

#define PVS_DEBUG			"../Graphics/Shaders/depth_debug.vert"
#define PFS_DEBUG			"../Graphics/Shaders/depth_debug.frag"
#define PVS_NAME			"../Graphics/Shaders/simpleVertexShader.vert"
#define PFS_NAME			"../Graphics/Shaders/simpleFragmentShader.frag"
#define PVS_TN				"../Graphics/Shaders/vShaderTN.vert"
#define PFS_TN				"../Graphics/Shaders/fShaderTN.frag"
#define PVS_LIGHT_NAME		"../Graphics/Shaders/light.vert"
#define PFS_LIGHT_NAME		"../Graphics/Shaders/light.frag"
#define PVS_SHADOW			"../Graphics/shadow_mapping_vs.vert"
#define PFS_SHADOW			"../Graphics/shadow_mapping_fs.frag"
#define PVS_SHADOW_DEPTH	"../Graphics/shadow_mapping_depth.vert"
#define PFS_SHADOW_DEPTH	"../Graphics/shadow_mapping_depth.frag"

/*----------------------------------------------------------------------------
MESHES
----------------------------------------------------------------------------*/

#define MESH_MONKEY			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/monkeyhead_smooth.dae"
#define MESH_PLANE			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/plane.dae"
#define MESH_AK				"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/ak.obj"
#define MESH_BUILDING		"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/building.dae"
#define MESH_DUDE			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/dude.dae"
#define MESH_CS				"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/terroristo.dae"
#define MESH_DEER			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/deer.dae"
#define MESH_BATEMAN		"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/juunioor.dae"
#define MESH_PENG			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/pengo.dae"
#define MESH_SPHERE			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/sphere.dae"
#define MESH_DEFAULT		"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/default_sphere.dae"
#define MESH_HEAD			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/snowmanHead.dae"
#define MESH_BRANCH			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/branch.dae"
#define MESH_VAMPIRE		"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/vamp/vampire.dae"
#define MESH_CYBORG			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/cyborg/cyborg.obj"
#define MESH_SNOWBALL		"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/snowball/snowball.obj"
#define MESH_NANOSUIT		"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/nanosuit/nanosuit.obj"
#define MESH_TERRAIN		"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/terrain/terrain.obj"
#define MESH_PLACEHOLDER    "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/map.dae"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

using namespace std;
Camera player_camera = Camera();
Camera light_view = Camera();

ShadowMapFBO shadow_fbo;

Shader lightShader, shadowShader, debugShader, showLightShader;

glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -40.0f, 60.f);
glm::vec3 lightPosition = glm::vec3(-1.0f,0.0f,0.0f);
glm::vec3 origin = glm::vec3(0.0f);
glm::mat4 lightSpaceMatrix;

bool isDebug = false;
bool isDepthMap = false;

GameObject terrain, light, sphere, character, bateman, default_sphere, shadow_light;
GameObject spheres[10];
std::vector<GameObject> Objects;
bool firstMouse = true;
float delta = 0.0f;
bool scalex = true;
bool scaley = true;
bool scalez = true;
bool scalexyz = true;

int width = 1920;
int height = 1080;
int shadowrez = 16000;
float maxCount = 10;
float counter = 0;
float shadowBias = 0.001f;
float shadowScale = 0.1f;

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

//void ShadowPass() {
//	shadow_fbo.BindForWriting();
//	glClear(GL_DEPTH_BUFFER_BIT);
//
//	m_shadowMapTech.Enable();
//
//	Matrix4f World = m_pMesh1->GetWorldMatrix();
//
//	Matrix4f LightView;
//	Vector3f Up(0.0f, 1.0f, 0.0f);
//	LightView.InitCameraTransform(m_spotLight.WorldPosition, m_spotLight.WorldDirection, Up);
//
//	Matrix4f WVP = m_lightPersProjMatrix * LightView * World;
//	m_shadowMapTech.SetWVP(WVP);
//
//	m_pMesh1->Render();
//}

unsigned int quadVAO = 0;
unsigned int quadVBO;
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
}

void LightPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shadow_fbo.BindForReading(GL_TEXTURE0);

	lightShader.Use();
	lightShader.SetInteger("shadowMap", 0, true);
	lightShader.SetMatrix4("proj", player_camera.GetProjection(), false);
	lightShader.SetMatrix4("view", player_camera.GetViewMatrix(), false);
	lightShader.SetVector3f("lightPos", lightPosition, false);
	lightShader.SetVector3f("viewPos", player_camera.Position, false);
	lightShader.SetFloat("biasFactor", shadowBias ,false);
	lightShader.SetMatrix4("lightSpaceMatrix", lightSpaceMatrix, false);
	for (int i = 0; i < Objects.size(); i++) {
		if (!Objects[i].isLight) {
			Objects[i].Draw(&lightShader);
		}
		else {
			Objects[i].Draw(&showLightShader);
		}
			
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

	counter += delta;
	if (counter > maxCount)
		counter = 0;

	float fraction = counter / maxCount;
	float radAngle = 2 * fraction * 3.1415;

	lightPosition = glm::vec3(cos(radAngle), sin(radAngle), 0.0f);
	glm::mat4 lightView = glm::lookAt(lightPosition, origin, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;

	glCullFace(GL_FRONT);
	ShadowPass();
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);


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

	LightPass();
	if(isDepthMap)
		Debug();

	glutSwapBuffers();
}



void updateScene() {
	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

	// Draw the next frame
	glutPostRedisplay();
}

void init()
{
	lightShader = Shader(PVS_SHADOW, PFS_SHADOW);
	showLightShader = Shader(PVS_LIGHT_NAME, PFS_LIGHT_NAME);
	//lightShader = Shader(PVS_NAME, PFS_NAME);
	shadowShader = Shader(PVS_SHADOW_DEPTH, PFS_SHADOW_DEPTH);
	debugShader = Shader(PVS_DEBUG, PFS_DEBUG);
	shadow_fbo.Init(shadowrez, shadowrez, false);
	player_camera.SetProjection(PERSP);
	light_view = Camera(6.15f, 5.0f, 3.93f, 0.0f, 1.0f, 1.0f, -121, -58);
	light_view.SetProjection(ORTHO);

	shadow_light = GameObject(MESH_DEFAULT);
	shadow_light.isLight = true;
	Objects.push_back(shadow_light);
	//default_sphere.Move(-10.f, 0.0f, 0.0f);

	//sphere = GameObject(MESH_SPHERE, PVS_NAME, PFS_NAME);
	//sphere.Move(5.0f, 5.0f, 5.0f);
	character = GameObject(MESH_PLACEHOLDER);
	character.Rotate(270.f, 0.0f, 0.0f);
	Objects.push_back(character);


	bateman = GameObject(MESH_BATEMAN);
	bateman.Scale(5.f, 5.f, 5.f);
	bateman.Move(5.f, 0.f, 0.0f);
	Objects.push_back(bateman);
	//Snowman.base = GameObject(MESH_SPHERE);
	//Snowman.arm = GameObject(MESH_SPHERE);
	//Snowman.head = GameObject(MESH_HEAD);
	//Snowman.branch_left = GameObject(MESH_BRANCH);
	//Snowman.branch_right = GameObject(MESH_BRANCH);
	//Objects.push_back(Snowman.base);
	//Objects.push_back(Snowman.arm);
	//Objects.push_back(Snowman.head);
	//Objects.push_back(Snowman.branch_left);
	//Objects.push_back(Snowman.branch_right);

	//light = GameObject(MESH_SPHERE, PVS_LIGHT_NAME, PFS_LIGHT_NAME);
	//light.Scale(0.1f, 0.1f, 0.1f);
	//light.Move(1.0f, 5.0f, 5.0f);

	//terrain = GameObject(MESH_TERRAIN);
	//terrain.Scale(10.0f, 10.0f, 10.0f);
	//terrain.Move(0.0f, -2.0f, 0.0f);
	//Objects.push_back(terrain);

	//for (int i = 0; i < 10; i++) {
	//	spheres[i] = GameObject(MESH_SPHERE);
	//	spheres[i].Move(cubePositions[i].x * 4, cubePositions[i].y * 2 + 5, cubePositions[i].z);
	//	spheres[i].Scale(cubePositions[i].x, cubePositions[i].x, cubePositions[i].x);
	//	Objects.push_back(spheres[i]);
	//}

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

	if (isDebug) {
		light_view.ProcessMouseMovement(xoffset, yoffset);
	}
	else {
		player_camera.ProcessMouseMovement(xoffset, yoffset);
	}
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
	switch (key)
	{
	case GLUT_KEY_UP:
		light_view.Position.x += 0.5f;
		break;
	case GLUT_KEY_DOWN:
		light_view.Position.x -= 0.5f;
		break;
	case GLUT_KEY_LEFT:
		light_view.Position.y -= 0.5f;
		break;
	case GLUT_KEY_RIGHT:
		light_view.Position.y += 0.5f;

		break;
	}
}

void keypress(unsigned char key, int x, int y) {

	int mod = glutGetModifiers();

	if (isDebug) {
		light_view.CameraKeyboard(key, mod, delta);
	}
	else {
		player_camera.CameraKeyboard(key,mod, delta);
	}

	if (key == 'r')
		player_camera.Reset();
	if (key == 't') {
		
		shadowBias += shadowBias;
		if (shadowBias == 0)
			shadowBias = 1;
		cout << "shadowBias: " << shadowBias << "\n";
	}
	if (key == 'g') {
		shadowBias -= shadowBias;
		if (shadowBias == 0)
			shadowBias = 1;
		cout << "shadowBias: " << shadowBias << "\n";
	}
	if (key == 'y') {
		shadowBias /= 0.1f;
		cout << "shadowBias: " << shadowBias << "\n";
	}
	if (key == 'h') {
		shadowBias *= 0.1f;
		cout << "shadowBias: " << shadowBias << "\n";
	}

	if (key == 'o')
		isDebug = !isDebug;
	if (key == 'p')
		isDepthMap = !isDepthMap;
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