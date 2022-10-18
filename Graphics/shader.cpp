#include "shader.h"

Shader& Shader::Use() {
	glUseProgram(ID);
	return *this;
}


void Shader::SetFloat(const char* name, float value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetInteger(const char* name, int value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetVector2f(const char* name, float x, float y, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::SetVector3f(const char* name, float x, float y, float z, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, &matrix[0][0]);
}

void Shader::Compile(const char* PVS, const char* PFS) {

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


unsigned int Shader::AddShader(const char* shaderText, GLenum shaderType)
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

void Shader::Delete()
{
	glDeleteProgram(ID);
}
