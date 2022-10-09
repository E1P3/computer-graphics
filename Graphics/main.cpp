#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"


// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Shader Shaders[2];
VAO _VAO[2];
VBO _VBO[2];
EBO _EBO[2];
int pointCount[2] = { 3, 3 };

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

static const char* pVS = readShaderSource("C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/default.vert");

// Fragment Shaders
static const char* pFS[] = { readShaderSource("C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/default.frag"), readShaderSource("C:/Users/HOW TO SPOON/Desktop/beans/code/computer-graphics/Graphics/flat_color.frag") };

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
	Shaders[0].Activate();
	glBindVertexArray(_VAO[0].ID);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	Shaders[1].Activate();
	glBindVertexArray(_VAO[1].ID);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}


void init()
{

	for (int i = 0; i < 2; i++)
	{
		// Set up the shaders
		Shaders[i].CompileVF(pVS, pFS[i]);
		_VAO[i].Init();
		_VAO[i].Bind();

		_VBO[i] = VBO(pointCount[i] * 7 * sizeof(float));
		_VBO[i].AddSubData(pointCount[i] * 3 * sizeof(GLfloat), (float*)vertices[i]);
		std::cout << _VBO[i].current_offset;
		_VBO[i].AddSubData(pointCount[i] * 4 * sizeof(GLfloat), (float*)colors);
		std::cout << _VBO[i].current_offset;
		_EBO[i] = EBO(indices, sizeof(indices));
		
		GLuint positionID = glGetAttribLocation(Shaders[i].ID, "vPosition");
		GLuint colorID = glGetAttribLocation(Shaders[i].ID, "vColor");

		std::cout << _VAO[i].ID;
		std::cout << _VBO[i].ID;
		std::cout << _EBO[i].ID;

		_VAO[i].LinkAttrib(_VBO[i], positionID, 3, GL_FLOAT, 0, 0);
		_VAO[i].LinkAttrib(_VBO[i], colorID, 4, GL_FLOAT, 0, BUFFER_OFFSET(pointCount[i] * 3 * sizeof(GLfloat)));

		_VAO[i].Unbind();
		_VBO[i].Unbind();
		_EBO[i].Unbind();
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