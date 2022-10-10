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


/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define MESH_NAME "C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/monkeyhead_smooth.dae"
#define PVS_NAME "C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Shaders/simpleVertexShader.vert"
#define PFS_NAME "C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Shaders/simpleFragmentShader.frag"
//#define MESH_NAME "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/ak.obj"

/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

using namespace std;
Camera player_camera;
MeshRenderer SceneRenderer;

int width = 1920;
int height = 1080;

GLfloat rotate_y = 0.0f;

void display() {

	GLuint ID = SceneRenderer.getShaderProgramID();

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ID);

	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(ID, "model");
	int view_mat_location = glGetUniformLocation(ID, "view");
	int proj_mat_location = glGetUniformLocation(ID, "proj");

	// Root of the Hierarchy
	mat4 view = identity_mat4();
	mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	mat4 model = identity_mat4();
	model = rotate_x_deg(model, 270.0);
	model = rotate_y_deg(model, rotate_y);

	view = rotate_z_deg(view, -player_camera.camera_X);
	view = rotate_x_deg(view, player_camera.camera_Y);
	//view = rotate_x_deg(view, );
	//view = rotate_x_deg(view, );
	view = translate(view, vec3(player_camera.camera_x, player_camera.camera_y, player_camera.camera_z));

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	glDrawArrays(GL_TRIANGLES, 0, SceneRenderer.getMeshPointCount());

	// Set up the child matrix
	mat4 modelChild = identity_mat4();
	//modelChild = rotate_z_deg(modelChild, 180);
	//modelChild = rotate_y_deg(modelChild, rotate_y);
	//modelChild = translate(modelChild, vec3(0.0f, 1.9f, 0.0f));

	// Apply the root matrix to the child matrix
	modelChild = model * modelChild;

	// Update the appropriate uniform and draw the mesh again
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild.m);
	glDrawArrays(GL_TRIANGLES, 0, SceneRenderer.getMeshPointCount());

	glutSwapBuffers();
}


void updateScene() {
	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

	if (player_camera.getKeyEnable()) {
		// Rotate the model slowly around the y axis at 20 degrees per second
		rotate_y += 120.0f * delta;
		rotate_y = fmodf(rotate_y, 360.0f);
	}

	// Draw the next frame
	glutPostRedisplay();
}

void init()
{
	SceneRenderer = MeshRenderer(MESH_NAME, PVS_NAME, PFS_NAME);
}

void mouseButton(int button, int state, int x, int y) {
	player_camera.MouseButton(button, state, x, y);
}

void mouseMove(int x, int y) {
	player_camera.MouseMove(x, y);
}

void keypress(unsigned char key, int x, int y) {
	player_camera.keypress(key, x, y);
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
	glutMouseFunc(mouseButton);
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
