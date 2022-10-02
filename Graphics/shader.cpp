#include "shader.h"

Shader::Shader() {
	ID = 0;
}

void Shader::getID() {
	ID = glCreateProgram();
	if (ID == 0)
	{
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}
}

void Shader::CompileAll() {
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

	glValidateProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &Success);
	if (Success == 0)
	{
		glGetProgramInfoLog(ID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}
}

void Shader::CompileVF(const char* PVS, const char* PFS) {

	this->getID();

	this->AddShader(PVS, GL_VERTEX_SHADER);
	this->AddShader(PFS, GL_FRAGMENT_SHADER);

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

	glValidateProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &Success);
	if (Success == 0)
	{
		glGetProgramInfoLog(ID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	this->Activate();

}


void Shader::AddShader(const char* shaderText, GLenum shaderType)
{
	// Create Shader Object and get its reference
	GLuint ShaderObj = glCreateShader(shaderType);
	if (ShaderObj == 0)
	{
		fprintf(stderr, "Error creating shader type %d\n", ShaderObj);
		exit(0);
	}
	// Attach Shader source to the Vertex Shader Object
	glShaderSource(ShaderObj, 1, (const GLchar**)&shaderText, NULL);
	// Compile the  Shader into machine code
	glCompileShader(ShaderObj);
	// Check for shader related errors using glGetShaderiv
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, InfoLog);
		exit(1);
	}

	// Attach the Shader to the Shader Program
	glAttachShader(ID, ShaderObj);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}
