#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

// USAGE
// you can either run all steps by assigning Vertex and Fragmant shader with CompileVF or you can getID, add as many shaders as you need with AddShaders, and then CompileAll

class Shader
{
public:
	// Reference ID of the Shader Program
	unsigned int ID;
	Shader() {}
	
	Shader(const char* PVS, const char* PFS) {
		this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS));
	}
	
	Shader& Use() {
		glUseProgram(ID);
		return *this;
	}


	void SetFloat(const std::string& name, float value, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
	}
	void SetInteger(const std::string& name, int value, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
	}
	void SetVector2f(const std::string& name, float x, float y, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);
	}
	void SetVector2f(const std::string& name, const glm::vec2& value, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y);
	}
	void SetVector3f(const std::string& name, float x, float y, float z, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
	}
	void SetVector3f(const std::string& name, const glm::vec3& value, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
	}
	void SetVector4f(const std::string& name, float x, float y, float z, float w, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
	}
	void SetVector4f(const std::string& name, const glm::vec4& value, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z, value.w);
	}
	void SetMatrix4(const std::string& name, const glm::mat4& matrix, bool useShader)
	{
		if (useShader)
			this->Use();
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, false, &matrix[0][0]);
	}

	void Compile(const char* PVS, const char* PFS) {

		if (!PVS || !PFS) {
			throw std::exception("SHADER_LINKING_ERROR: Shader data is empty");
		}

		ID = glCreateProgram();
		if (ID == 0)
		{
			fprintf(stderr, "Error creating shader program\n");
			exit(1);
		}

		unsigned int vertex, fragment;

		vertex = this->AddShader(PVS, GL_VERTEX_SHADER);
		fragment = this->AddShader(PFS, GL_FRAGMENT_SHADER);

		GLint Success = 0;
		GLchar ErrorLog[1024] = { 0 };

		// After compiling all shader objects and attaching them to the program, we can finally link it
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &Success);
		if (Success == 0)
		{
			glGetProgramInfoLog(ID, sizeof(ErrorLog), NULL, ErrorLog);
			fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
			exit(1);
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}



	void Delete()
	{
		glDeleteProgram(ID);
	}


private:

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

	unsigned int AddShader(const char* shaderText, GLenum shaderType)
	{
		// Create Shader Object and get its reference
		unsigned int ShaderObj = glCreateShader(shaderType);
		if (ShaderObj == 0)
		{
			fprintf(stderr, "Error creating shader type %d\n", ShaderObj);
			exit(0);
		}
		// Attach Shader source to the Vertex Shader Object
		glShaderSource(ShaderObj, 1, (const char**)&shaderText, NULL);
		// Compile the  Shader into machine code
		glCompileShader(ShaderObj);
		// Check for shader related errors using glGetShaderiv
		GLint success;
		glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char InfoLog[1024];
			glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
			fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, InfoLog);
			exit(1);
		}

		// Attach the Shader to the Shader Program
		glAttachShader(ID, ShaderObj);
		return ShaderObj;
	}
};


#endif