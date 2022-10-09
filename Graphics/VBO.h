#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <GL/glew.h>
#include <GL/freeglut.h>

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	int current_offset;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO();
	VBO(GLsizeiptr size);
	VBO(GLsizeiptr size, const void* data);

	void AddSubData(int size, const void* data);
	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

#endif