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
#include "shader.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"


/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define MESH_NAME "C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Meshes/monkeyhead_smooth.dae"
//#define MESH_NAME "C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Meshes/ak.obj"

/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

#pragma region SimpleTypes
typedef struct ModelData
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;
#pragma endregion SimpleTypes

using namespace std;
GLuint shaderProgramID;
Shader mesh_shader;
Camera player_camera;

ModelData mesh_data;
int width = 1920;
int height = 1080;

GLuint loc1, loc2, loc3;
GLfloat rotate_y = 0.0f;

#pragma region MESH LOADING
/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

ModelData load_mesh(const char* file_name) {
	ModelData modelData;

	/* Use assimp to read the model file, forcing it to be read as    */
	/* triangles. The second flag (aiProcess_PreTransformVertices) is */
	/* relevant if there are multiple meshes in the model file that   */
	/* are offset from the origin. This is pre-transform them so      */
	/* they're in the right position.                                 */
	const aiScene* scene = aiImportFile(
		file_name,
		aiProcess_Triangulate
	);

	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return modelData;
	}

	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf("    %i vertices in mesh\n", mesh->mNumVertices);
		modelData.mPointCount += mesh->mNumVertices;
		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
			if (mesh->HasPositions()) {
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				modelData.mVertices.push_back(vec3(vp->x, vp->y, vp->z));
			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				modelData.mNormals.push_back(vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				modelData.mTextureCoords.push_back(vec2(vt->x, vt->y));
			}
			if (mesh->HasTangentsAndBitangents()) {
				/* You can extract tangents and bitangents here              */
				/* Note that you might need to make Assimp generate this     */
				/* data for you. Take a look at the flags that aiImportFile  */
				/* can take.                                                 */
			}
		}
	}

	aiReleaseImport(scene);
	return modelData;
}

#pragma endregion MESH LOADING

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS
char* readShaderSource(const char* shaderFile) {
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}

void display() {
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(mesh_shader.ID);

	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(mesh_shader.ID, "model");
	int view_mat_location = glGetUniformLocation(mesh_shader.ID, "view");
	int proj_mat_location = glGetUniformLocation(mesh_shader.ID, "proj");

	// Root of the Hierarchy
	mat4 view = identity_mat4();
	mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	mat4 model = identity_mat4();
	model = rotate_x_deg(model, 270.0);
	model = rotate_y_deg(model, rotate_y);

	view = rotate_y_deg(view, player_camera.camera_X);
	view = rotate_x_deg(view, player_camera.camera_Y);
	//view = rotate_x_deg(view, );
	//view = rotate_x_deg(view, );
	view = translate(view, vec3(player_camera.camera_x, player_camera.camera_y, player_camera.camera_z));

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);

	// Set up the child matrix
	mat4 modelChild = identity_mat4();
	//modelChild = rotate_z_deg(modelChild, 180);
	//modelChild = rotate_y_deg(modelChild, rotate_y);
	//modelChild = translate(modelChild, vec3(0.0f, 1.9f, 0.0f));

	// Apply the root matrix to the child matrix
	modelChild = model * modelChild;

	// Update the appropriate uniform and draw the mesh again
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild.m);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);

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
		rotate_y += 20.0f * delta;
		rotate_y = fmodf(rotate_y, 360.0f);
	}

	// Draw the next frame
	glutPostRedisplay();
}

void LoadObject(Shader& _shader, ModelData _mesh_data, const char* _PVS, const char* _PFS) {
	_shader.Init();
	_shader.AddShader(_PVS, GL_VERTEX_SHADER);
	_shader.AddShader(_PFS, GL_FRAGMENT_SHADER);
	_shader.CompileAll();

	loc1 = glGetAttribLocation(mesh_shader.ID, "vertex_position");
	loc2 = glGetAttribLocation(mesh_shader.ID, "vertex_normal");
	loc3 = glGetAttribLocation(mesh_shader.ID, "vertex_texture");

	VBO mesh_vbo_v = VBO(mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0]);
	VBO mesh_vbo_f = VBO(mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0]);
	VAO mesh_vao;

	mesh_vao.Init();
	mesh_vao.LinkAttrib(mesh_vbo_v, loc1, 3, GL_FLOAT, 0, 0);
	mesh_vao.LinkAttrib(mesh_vbo_f, loc2, 3, GL_FLOAT, 0, 0);
}

void init()
{
	mesh_data = load_mesh(MESH_NAME);
	const char* PVS = readShaderSource("C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Shaders/simpleVertexShader.vert");
	const char* PFS = readShaderSource("C:/Users/jansz/Desktop/beans/programming_stuff/computer-graphics/Graphics/Shaders/simpleFragmentShader.frag");
	LoadObject(mesh_shader, mesh_data, PVS, PFS);
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
