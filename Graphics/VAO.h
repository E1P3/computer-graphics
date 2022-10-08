#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include"VBO.h"

class VAO
{
public:
	// ID reference for the Vertex Array Object
	GLuint ID;
	// Constructor that generates a VAO ID
	VAO();

	void Init();

	// Links a VBO to the VAO using a certain layout
	void LinkVBO(VBO& VBO, GLuint layout);

	void LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Binds the VAO
	void Bind();
	// Unbinds the VAO
	void Unbind();
	// Deletes the VAO
	void Delete();
};
#endif