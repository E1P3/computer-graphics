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
#define MESH_PLANE "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/plane.dae"
#define PVS_NAME "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Shaders/simpleVertexShader.vert"
#define PFS_NAME "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Shaders/simpleFragmentShader.frag"
#define MESH_AK "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/ak.obj"
#define MESH_BUILDING "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/building.dae"
#define MESH_DUDE "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/dude.dae"
#define MESH_CS "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/terroristo.dae"
#define MESH_DEER "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/deer.dae"
#define MESH_PLANE "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/plane.dae"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

using namespace std;
Camera player_camera = Camera(glm::vec3(0.0f,1.0f,1.0f));
GameObject obj1, obj2;
bool firstMouse = true;
float delta = 0.0f;
bool scalex = true;
bool scaley = true;
bool scalez = true;
bool scalexyz = true;

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
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//printf("Frame \n");
	//std::cout << "CAMERA: " << player_camera.Position.x << ", " << player_camera.Position.y << ", " << player_camera.Position.z << ", YAW: " << player_camera.Yaw << " PITCH: " << player_camera.Pitch << "\n";

	
	//obj1.Renderer.transform = glm::rotate(obj2.Renderer.transform, 90.0f, glm::vec3(1.f, 0.f, 1.f));

	obj1.Renderer.Draw(player_camera.GetProjection(), player_camera.GetViewMatrix());
	obj2.Renderer.Draw(player_camera.GetProjection(), player_camera.GetViewMatrix());


	//cout << "FPS: " << 1 / delta << "\n";

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

	obj1 = GameObject();
	obj1.RenderSetup(MESH_DUDE, PVS_NAME, PFS_NAME);
	
	obj2 = GameObject();
	obj2.RenderSetup(MESH_PLANE, PVS_NAME, PFS_NAME);
	obj2.Renderer.Rotate(270.0f, 0.0f, 0.0f);
	obj2.Renderer.Scale(10.0f, 10.0f, 10.0f);

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

void arrowKeyes(int key, int x, int y) {
	switch (key)
	{
		case GLUT_KEY_UP:
			obj1.Renderer.SetRotation(0.0f, 180.0f, 0.0f);
			obj1.Renderer.Move(0.0f, 0.0f, -0.05f);
			break;
		case GLUT_KEY_DOWN:
			obj1.Renderer.SetRotation(0.0f, 0.0f, 0.0f);
			obj1.Renderer.Move(0.0f, 0.0f, 0.05f);
			break;
		case GLUT_KEY_LEFT:
			obj1.Renderer.SetRotation(0.0f, 270.0f, 0.0f);
			obj1.Renderer.Move(-0.05f, 0.0f, 0.0f);
			break;
		case GLUT_KEY_RIGHT:
			obj1.Renderer.SetRotation(0.0f, 90.0f, 0.0f);
			obj1.Renderer.Move(0.05f, 0.0f, 0.0f);
			
			break;
	}
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
	if (key == 'o')
		player_camera.SetProjection(ORTHO);
	if (key == 'p')
		player_camera.SetProjection(PERSP);
	if (key == 'z') {
		if (scalex)
			obj1.Renderer.Scale(2.0f, 1.0f, 1.0f);
		else
			obj1.Renderer.Scale(1.0f, 1.0f, 1.0f);
		scalex = !scalex;
	}
	if (key == 'x') {
		if (scaley)
			obj1.Renderer.Scale(1.0f, 2.0f, 1.0f);
		else
			obj1.Renderer.Scale(1.0f, 1.0f, 1.0f);
		scaley = !scaley;
	}
	if (key == 'c') {
		if (scalez)
			obj1.Renderer.Scale(1.0f, 1.0f, 2.0f);
		else
			obj1.Renderer.Scale(1.0f, 1.0f, 1.0f);
		scalez = !scalez;
	}
	if (key == 'v') {
		if (scalexyz)
			obj1.Renderer.Scale(10.0f, 10.0f, 10.0f);
		else
			obj1.Renderer.Scale(1.0f, 1.0f, 1.0f);
		scalexyz = !scalexyz;
	}
	if (key == 'b')
		obj1.Renderer.Rotate(45.0f, 0.f, 0.f);
	if (key == 'n')
		obj1.Renderer.Rotate(0.f, 45.0f, 0.f);
	if (key == 'm')
		obj1.Renderer.Rotate(0.f, 0.f, 45.0f);
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
	glutSpecialFunc(arrowKeyes);

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
