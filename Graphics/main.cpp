#include <glm/glm.hpp>
#include <iostream>
#include <math.h>
#include <vector>

//Shader functions
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include "shader.h"
#include "maths_funcs.h"

//OpenGl
#include <GL/glew.h>
#include <GL/freeglut.h>

//Assimp
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Shader Shaders[2];
VAO _VAO[2];
VBO _VBO[2];
EBO _EBO[2];
int pointCount[2] = { 3, 3 };

typedef struct ModelData
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;

char* readShaderSource(const char* shaderFile) {

	std::cout << "Reading Shader file " << shaderFile << "...\n";
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb");

	if (fp == NULL) {
		std::cout << "Shader file not found\n";
		return NULL;
	}

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);
	std::cout << "Shader file loaded \n";
	return buf;
}

static const char* pVS = readShaderSource("C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Shaders/default.vert");

// Fragment Shaders
static const char* pFS[] = { readShaderSource("C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Shaders/default.frag"), readShaderSource("C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/Shaders/flat_color.frag") };

// Create 2 sets of 3 vertices to make up 2 triangles that fits on the viewport
glm::vec3 vertices[][3] =
{
	{
		glm::vec3(-0.5f,  0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f)
	},
	{
		glm::vec3(-0.5f,  0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f,  0.5f, 0.0f)
	}
};

// Create a color array that identfies the colors of each vertex (format R, G, B, A)
// Note: The color array will be ignored by the fragment shader for the second triangle
glm::vec4 colors[] =
{
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
};

// Create an index buffer for the 2 triangles
GLuint indices[] =
{
		0, 1, 2
};

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	// NB: Make the call to draw the geometry in the currently activated vertex buffer. This is where the GPU starts to work!
	// Need to call shader here
	for (int i = 0; i < 2; i++) {

		std::cout << "Using properties \n";
		std::cout << "Shader: " << Shaders[i].ID << "\n";
		std::cout << "VAO: " << _VAO[i].ID << "\n";

		Shaders[i].Activate();
		glBindVertexArray(_VAO[i].ID);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}

	glutSwapBuffers();
}


void LoadObject(VAO& _VAO, VBO& _VBO, EBO& _EBO, Shader& _Shader, const char* PVS, const char* PFS, float* _vertices, float* _colors, GLuint indicies[], int _pointCount) {

	std::cout << "Loading Object\n";

	_Shader.CompileVF(PVS, PFS);
	_VAO.Init();
	_VAO.Bind();

	_VBO = VBO(_pointCount * 7 * sizeof(float));
	_VBO.AddSubData(_pointCount * 3 * sizeof(GLfloat), _vertices);
	_VBO.AddSubData(_pointCount * 4 * sizeof(GLfloat), _colors);
	_EBO = EBO(indices, sizeof(indices));

	GLuint positionID = glGetAttribLocation(_Shader.ID, "vPosition");
	GLuint colorID = glGetAttribLocation(_Shader.ID, "vColor");

	_VAO.LinkAttrib(_VBO, positionID, 3, GL_FLOAT, 0, 0);
	_VAO.LinkAttrib(_VBO, colorID, 4, GL_FLOAT, 0, BUFFER_OFFSET(_pointCount * 3 * sizeof(GLfloat)));

	_VAO.Unbind();
	_VBO.Unbind();
	_EBO.Unbind();

	std::cout << "Object Loaded \n";
	std::cout << "VAO: " << _VAO.ID << "\n";
	std::cout << "VBO: " << _VBO.ID << "\n";
	std::cout << "EBO: " << _EBO.ID << "\n";
	std::cout << "Shader: " << _Shader.ID << "\n";

}

void init()
{

	for (int i = 0; i < 2; i++) {
		LoadObject(_VAO[i], _VBO[i], _EBO[i], Shaders[i], pVS, pFS[i], (float*)vertices[i], (float*)colors, indices, pointCount[i]);
	}

}

int main(int argc, char** argv)
{
	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Lab 1");
	// Tell glut where the display function is
	glutDisplayFunc(display);

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