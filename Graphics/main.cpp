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
#include "Cubemap.h"
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
#define PVS_SHADOW			"../Graphics/Shaders/shadow_mapping_vs.vert"
#define PFS_SHADOW			"../Graphics/Shaders/shadow_mapping_fs.frag"
#define PVS_SHADOW_DEPTH	"../Graphics/Shaders/shadow_mapping_depth.vert"
#define PFS_SHADOW_DEPTH	"../Graphics/Shaders/shadow_mapping_depth.frag"
#define PVS_SKYBOX			"../Graphics/Shaders/skybox.vert"
#define PFS_SKYBOX			"../Graphics/Shaders/skybox.frag"

/*----------------------------------------------------------------------------
MESHES
----------------------------------------------------------------------------*/
#define MESH_DEFAULT		"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/default.dae"
#define MESH_BATEMAN		"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/bateman.dae"
#define MESH_PLACEHOLDER    "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/map.dae"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

using namespace std;
Camera player_camera = Camera();
Camera light_view = Camera();

ShadowMapFBO shadow_fbo;
Cubemap skybox;
Shader lightShader, shadowShader, debugShader, skyboxShader;

glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -60.0f, 60.f);
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
float shadowBias = 0.0000001f;
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

void SkyPass() {
	skyboxShader.Use();
	glm::mat4 view = glm::mat4(glm::mat3(player_camera.GetViewMatrix()));
	skyboxShader.SetMatrix4("view", view, false);
	skyboxShader.SetMatrix4("proj", player_camera.GetProjection(), false);
	skybox.Draw();
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
		Objects[i].Draw(&lightShader);
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
	if (!isDebug)
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

	LightPass();
	if(isDepthMap)
		Debug();
	SkyPass();
	glutSwapBuffers();

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
	skyboxShader = Shader(PVS_SKYBOX, PFS_SKYBOX);
	lightShader = Shader(PVS_SHADOW, PFS_SHADOW);
	shadowShader = Shader(PVS_SHADOW_DEPTH, PFS_SHADOW_DEPTH);
	debugShader = Shader(PVS_DEBUG, PFS_DEBUG);

	shadow_fbo.Init(shadowrez, shadowrez, false);

	player_camera.SetProjection(PERSP);

	skybox = Cubemap(
		vector<std::string>
		{
			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Textures/skybox/clouds_right.bmp",
			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Textures/skybox/clouds_left.bmp",
			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Textures/skybox/clouds_top.bmp",
			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Textures/skybox/clouds_bottom.bmp",
			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Textures/skybox/clouds_front.bmp",
			"C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Textures/skybox/clouds_back.bmp"
		});

	character = GameObject(MESH_PLACEHOLDER);
	character.Rotate(270.f, 0.0f, 0.0f);
	Objects.push_back(character);
	bateman = GameObject(MESH_BATEMAN);
	Objects.push_back(bateman);

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
	switch (key)
	{
	case GLUT_KEY_UP:

		break;
	case GLUT_KEY_DOWN:

		break;
	case GLUT_KEY_LEFT:

		break;
	case GLUT_KEY_RIGHT:


		break;
	}
}

void keypress(unsigned char key, int x, int y) {

	int mod = glutGetModifiers();


	player_camera.CameraKeyboard(key,mod, delta);

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