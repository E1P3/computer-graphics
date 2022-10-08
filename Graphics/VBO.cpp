#include"VBO.h"

VBO::VBO() {
	ID = 0;
	current_offset = 0;
}

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(GLsizeiptr size)
{
	current_offset = 0;
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
}

void VBO::AddSubData(int size, GLfloat data[]) {
	this->Bind();
	glBufferSubData(GL_ARRAY_BUFFER, current_offset, size, data);
	current_offset += size;
	this->Unbind();
}

// Binds the VBO
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}