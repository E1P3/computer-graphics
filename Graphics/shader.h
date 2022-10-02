#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include<iostream>

// USAGE
// you can either run all steps by assigning Vertex and Fragmant shader with CompileVF or you can getID, add as many shaders as you need with AddShaders, and then CompileAll

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	Shader();
	// Constructor that build the Shader Program from 2 different shaders
	void AddShader(const char* shaderText, GLenum shaderType);
	void CompileAll();
	void CompileVF(const char* PVS, const char* PFS);
	void getID();
	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

};
#endif