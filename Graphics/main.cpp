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

static const char* pVS = "                                                   \n\
#version 330                                                                 \n\
                                                                             \n\
in vec3 vPosition;															 \n\
in vec4 vColor;																 \n\
																			 \n\
out vec4 color;																 \n\
                                                                             \n\
void main()                                                                  \n\
{                                                                            \n\
	gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);			 \n\
	color = vColor;							                                 \n\
}";

// Fragment Shaders
static const char* pFS[] = { "                                               \n\
#version 330                                                                 \n\
																			 \n\
in vec4 color;                                                               \n\
out vec4 FragColor;                                                          \n\
                                                                             \n\
void main()                                                                  \n\
{                                                                            \n\
FragColor = color;									                         \n\
}",
"																			 \n\
#version 330                                                                 \n\
																			 \n\
in vec4 color;                                                               \n\
out vec4 FragColor;                                                          \n\
                                                                             \n\
void main()                                                                  \n\
{                                                                            \n\
FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);									 \n\
}"
};

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

// _VBO Functions - click on + to expand
#pragma region _VBO_FUNCTIONS
void generateObjectBuffer(GLuint& _VBO, GLfloat vertices[], GLfloat colors[])
{
	GLuint numVertices = 3;
	// Genderate 1 generic buffer object, called _VBO
	glGenBuffers(1, &_VBO);
	// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
	// Buffer will contain an array of vertices 
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
	glBufferData(GL_ARRAY_BUFFER, numVertices * 7 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	// If you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), numVertices * 4 * sizeof(GLfloat), colors);
}

void linkCurrentBuffertoShader(GLuint& _VAO, GLuint& _VBO, GLuint shaderProgramID)
{
	GLuint numVertices = 3;
	// Find the location of the variables that we will be using in the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
	GLuint colorID = glGetAttribLocation(shaderProgramID, "vColor");
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	// Have to enable this
	glEnableVertexAttribArray(positionID);
	// Tell it where to find the position data in the currently active buffer (at index positionID)
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Similarly, for the color data.
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numVertices * 3 * sizeof(GLfloat)));
}
#pragma endregion _VBO_FUNCTIONS


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

		_VBO[i] = VBO(3 * 7 * sizeof(float));
		_VBO[i].AddSubData(3 * 3 * sizeof(GLfloat), (float*)vertices[i]);
		std::cout << _VBO[i].current_offset;
		_VBO[i].AddSubData(3 * 4 * sizeof(GLfloat), (float*)colors);
		std::cout << _VBO[i].current_offset;
		_EBO[i] = EBO(indices, sizeof(indices));
		
		GLuint positionID = glGetAttribLocation(Shaders[i].ID, "vPosition");
		GLuint colorID = glGetAttribLocation(Shaders[i].ID, "vColor");

		std::cout << _VAO[i].ID;
		std::cout << _VBO[i].ID;
		std::cout << _EBO[i].ID;

		_VAO[i].LinkAttrib(_VBO[i], positionID, 3, GL_FLOAT, 0, 0);
		_VAO[i].LinkAttrib(_VBO[i], colorID, 4, GL_FLOAT, 0, BUFFER_OFFSET(3 * 3 * sizeof(GLfloat)));

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