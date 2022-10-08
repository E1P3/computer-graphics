#include"VAO.h"

// Constructor that generates a VAO ID
VAO::VAO()
{
	ID = 0;
}

void VAO::Init() {
	glGenVertexArrays(1, &ID);
}

// Links a VBO to the VAO using a certain layout
void VAO::LinkVBO(VBO& VBO, GLuint layout)
{
	VBO.Bind();
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	VBO.Unbind();
}

void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	VBO.Unbind();
}


// Binds the VAO
void VAO::Bind()
{
	glBindVertexArray(ID);
}

// Unbinds the VAO
void VAO::Unbind()
{
	glBindVertexArray(0);
}

// Deletes the VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}