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
#include "MeshRenderer.h"
#include "GameObject.h"


/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define MESH_MONKEY "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/monkeyhead_smooth.dae"
#define MESH_PLANE "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/plane.dae"
#define PVS_NAME "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Shaders/simpleVertexShader.vert"
#define PFS_NAME "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Shaders/simpleFragmentShader.frag"
#define PVS_LIGHT_NAME "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Shaders/light.vert"
#define PFS_LIGHT_NAME "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Shaders/light.frag"
#define MESH_AK "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/ak.obj"
#define MESH_BUILDING "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/building.dae"
#define MESH_DUDE "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/dude.dae"
#define MESH_CS "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/terroristo.dae"
#define MESH_DEER "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/deer.dae"
#define MESH_BATEMAN "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/juunioor.dae"
#define MESH_PENG "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/pengo.dae"
#define MESH_SPHERE "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/sphere.dae"
#define MESH_HEAD "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/snowmanHead.dae"
#define MESH_BRANCH "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/branch.dae"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

using namespace std;
Camera player_camera = Camera();
GameObject terrain, light, sphere;
GameObject spheres[10];
bool firstMouse = true;
float delta = 0.0f;
bool scalex = true;
bool scaley = true;
bool scalez = true;
bool scalexyz = true;

int width = 1920;
int height = 1080;
float counter = 0;

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

void display() {
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//printf("Frame \n");
	std::cout << "CAMERA: " << player_camera.Position.x << ", " << player_camera.Position.y << ", " << player_camera.Position.z << ", YAW: " << player_camera.Yaw << " PITCH: " << player_camera.Pitch << "\n";

	
	//obj1.Renderer.transform = glm::rotate(obj2.Renderer.transform, 90.0f, glm::vec3(1.f, 0.f, 1.f));

	counter += delta;
	if (counter > 100)
		counter = 0;


	local1 = glm::mat4(1.0f);
	local1 = glm::rotate(sin(counter) * 20.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * local1;
	local1 = glm::translate(0.0f, 1.0f, 0.0f) * local1;
	Snowman.base.Renderer.transform = local1 * glm::scale(1.5f, 1.5f, 1.5f);

	local2 = glm::mat4(1.0f);
	local2 = glm::rotate(sin(counter) * 20.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * local2;
	local2 = glm::translate(0.0f, 1.5f, 0.0f) * local2;
	Snowman.arm.Renderer.transform = local1 * local2 * glm::scale(1.2f, 1.2f, 1.2f);;

	local21 = glm::mat4(1.0f);
	local21 = glm::rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(-sin(counter) * 40.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * local21;
	local21 = glm::translate(1.0f, 0.5f, 0.0f) * local21;
	Snowman.branch_right.Renderer.transform = local1 * local2 * local21 * glm::scale(0.2f, 0.2f, 0.2f);

	local22 = glm::mat4(1.0f);
	local22 = glm::rotate(270.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(sin(counter) * 40.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * local22;
	local22 = glm::translate(-1.0f, 0.5f, 0.0f) * local22;
	Snowman.branch_left.Renderer.transform = local1 * local2 * local22 * glm::scale(0.2f, 0.2f, 0.2f);


	local3 = glm::mat4(1.0f);
	local3 = glm::rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f))* glm::rotate(270.0f, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(sin(counter) * 10.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * local3;
	local3 = glm::translate(0.0f, 1.5f, 0.0f) * local3;
	Snowman.head.Renderer.transform = local1 * local2 * local3 * glm::scale(0.4f, 0.4f, 0.4f);
	
	terrain.Renderer.Draw(player_camera.GetProjection(), player_camera.GetViewMatrix(),player_camera.Position, light.Renderer.position);
	light.Renderer.Draw(player_camera.GetProjection(), player_camera.GetViewMatrix(), player_camera.Position, light.Renderer.position);
	Snowman.base.Renderer.Draw(player_camera.GetProjection(), player_camera.GetViewMatrix(), player_camera.Position, light.Renderer.position);
	Snowman.arm.Renderer.Draw(player_camera.GetProjection(), player_camera.GetViewMatrix(), player_camera.Position, light.Renderer.position);
	Snowman.branch_right.Renderer.Draw(player_camera.GetProjection(), player_camera.GetViewMatrix(), player_camera.Position, light.Renderer.position);
	Snowman.branch_left.Renderer.Draw(player_camera.GetProjection(), player_camera.GetViewMatrix(), player_camera.Position, light.Renderer.position);
	Snowman.head.Renderer.Draw(player_camera.GetProjection(), player_camera.GetViewMatrix(), player_camera.Position, light.Renderer.position);

	sphere.Renderer.Draw(player_camera.GetProjection(), player_camera.GetViewMatrix(), player_camera.Position, light.Renderer.position);
	//cout << "FPS: " << 1 / delta << "\n";

	for (int i = 0; i < 10; i++) {
		spheres[i].Renderer.Draw(player_camera.GetProjection(), player_camera.GetViewMatrix(), player_camera.Position, light.Renderer.position);
	}

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
	player_camera.SetProjection(PERSP);

	sphere = GameObject();
	sphere.RenderSetup(MESH_SPHERE, PVS_NAME, PFS_NAME);
	sphere.Renderer.Move(5.0f, 5.0f, 5.0f);

	Snowman.base = GameObject();
	Snowman.base.RenderSetup(MESH_SPHERE, PVS_NAME, PFS_NAME);

	Snowman.arm = GameObject();
	Snowman.arm.RenderSetup(MESH_SPHERE, PVS_NAME, PFS_NAME);

	Snowman.head = GameObject();
	Snowman.head.RenderSetup(MESH_HEAD, PVS_NAME, PFS_NAME);

	Snowman.branch_left = GameObject();
	Snowman.branch_left.RenderSetup(MESH_BRANCH, PVS_NAME, PFS_NAME);

	Snowman.branch_right = GameObject();
	Snowman.branch_right.RenderSetup(MESH_BRANCH, PVS_NAME, PFS_NAME);

	light = GameObject();
	light.RenderSetup(MESH_SPHERE, PVS_LIGHT_NAME, PFS_LIGHT_NAME);
	light.Renderer.Scale(0.1f, 0.1f, 0.1f);
	light.Renderer.Move(1.0f, 5.0f, 5.0f);

	terrain = GameObject();
	terrain.RenderSetup(MESH_PLANE, PVS_NAME, PFS_NAME);
	terrain.Renderer.Rotate(270.0f, 0.0f, 0.0f);
	terrain.Renderer.Scale(1000.0f, 1000.0f, 1000.0f);

	for (int i = 0; i < 10; i++) {
		spheres[i] = GameObject();
		spheres[i].RenderSetup(MESH_SPHERE, PVS_NAME, PFS_NAME);
		spheres[i].Renderer.Move(cubePositions[i].x * 4 , cubePositions[i].y * 2 + 5, cubePositions[i].z);
		spheres[i].Renderer.Scale(cubePositions[i].x , cubePositions[i].x , cubePositions[i].x );
	}

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
			light.Renderer.SetRotation(0.0f, 180.0f, 0.0f);
			light.Renderer.Move(0.0f, 0.0f, -0.1f);
			break;
		case GLUT_KEY_DOWN:
			light.Renderer.SetRotation(0.0f, 0.0f, 0.0f);
			light.Renderer.Move(0.0f, 0.0f, 0.1f);
			break;
		case GLUT_KEY_LEFT:
			light.Renderer.SetRotation(0.0f, 270.0f, 0.0f);
			light.Renderer.Move(-0.1f, 0.0f, 0.0f);
			break;
		case GLUT_KEY_RIGHT:
			light.Renderer.SetRotation(0.0f, 90.0f, 0.0f);
			light.Renderer.Move(0.1f, 0.0f, 0.0f);
			
			break;
	}
}

void keypress(unsigned char key, int x, int y) {

	int mod = glutGetModifiers();

	if (mod == GLUT_ACTIVE_SHIFT)
		light.Renderer.Move(0.0f, 0.1f, 0.0f);

	if (mod == GLUT_ACTIVE_CTRL)
		light.Renderer.Move(0.0f, -0.1f, 0.0f);

	if(key == 'w')
		player_camera.ProcessKeyboard(FORWARD, delta);
	if(key == 's')
		player_camera.ProcessKeyboard(BACKWARD, delta);			
	if(key == 'a')
		player_camera.ProcessKeyboard(LEFT, delta);			
	if(key == 'd')
		player_camera.ProcessKeyboard(RIGHT, delta);			
	if(key == 'r')
		player_camera.Reset();
	if (key == 't')
		light.Renderer.Move(0.0f, 0.1f, 0.0f);
	if (key == 'g')
		light.Renderer.Move(0.0f, -0.1f, 0.0f);
	if (key == 'o')
		player_camera.SetProjection(ORTHO);
	if (key == 'p')
		player_camera.SetProjection(PERSP);
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
