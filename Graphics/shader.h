#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// USAGE
// you can either run all steps by assigning Vertex and Fragmant shader with CompileVF or you can getID, add as many shaders as you need with AddShaders, and then CompileAll

class Shader
{
public:
	// Reference ID of the Shader Program
	unsigned int ID;
	Shader() {}
	Shader& Use();
	void Delete();
	void Compile(const char* PVS, const char* PFS);
	void SetFloat(const char* name, float value, bool useShader = false);
	void SetInteger(const char* name, int value, bool useShader = false);
	void SetVector2f(const char* name, float x, float y, bool useShader = false);
	void SetVector2f(const char* name, const glm::vec2& value, bool useShader = false);
	void SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
	void SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
	void SetVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
	void SetVector4f(const char* name, const glm::vec4& value, bool useShader = false);
	void SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);

private:
	unsigned int AddShader(const char* shaderText, GLenum shaderType);
};
#endif