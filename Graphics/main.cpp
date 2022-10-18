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
#include "maths_funcs.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "GameObject.h"


/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define MESH_MONKEY "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/monkeyhead_smooth.dae"
#define PVS_NAME "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Shaders/simpleVertexShader.vert"
#define PFS_NAME "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Shaders/simpleFragmentShader.frag"
#define MESH_AK "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/ak.obj"

/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

using namespace std;
Camera player_camera;
GameObject obj1, obj2;
bool firstMouse = true;
float delta = 0.0f;

int width = 1920;
int height = 1080;

float lastX = width / 2.0f;
float lastY = height / 2.0f;

GLfloat rotate_y = 0.0f;

glm::vec3 cubePositions[] = {
	   glm::vec3(0.0f,  0.0f,  0.0f),
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//printf("Frame \n");
	std::cout << "CAMERA: " << player_camera.Position.x << ", " << player_camera.Position.y << ", " << player_camera.Position.z << ", YAW: " << player_camera.Yaw << " PITCH: " << player_camera.Pitch << "\n";

	obj1.Draw();
	obj2.Draw();

	cout << "FPS: " << 1 / delta << "\n";

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

	obj1 = GameObject(0.0f, 0.0f, 0.0f, width, height);
	obj1.SetCamera(&player_camera);
	obj1.RenderSetup(MESH_MONKEY, PVS_NAME, PFS_NAME);
	
	obj2 = GameObject(-100.0f, 0.0f, -100.0f, width, height);
	obj2.SetCamera(&player_camera);
	obj2.RenderSetup(MESH_AK, PVS_NAME, PFS_NAME);
	
}

void mouseMove(int x, int y) {
	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xoffset = x - lastX;
	float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top

	lastX = x;
	lastY = y;

	player_camera.ProcessMouseMovement(xoffset, yoffset);
}

void keypress(unsigned char key, int x, int y) {

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
}
int main(int argc, char** argv) {
	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Hello Triangle");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	glutMotionFunc(mouseMove);

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
