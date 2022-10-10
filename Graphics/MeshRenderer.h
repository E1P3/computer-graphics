#ifndef MESHRENDERER_CLASS_H
#define MESHRENDERER_CLASS_H

#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

#include "maths_funcs.h"
#include "shader.h"
#include "VAO.h"
#include "VBO.h"

class MeshRenderer
{

typedef struct ModelData
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;

private:
	Shader mesh_shader;
	ModelData mesh_data;
	const char* PVS;
	const char* PFS;
	char* readShaderSource(const char* shaderFile);

public:
	MeshRenderer();
	MeshRenderer(const char* mesh_file_name, const char* pvs_file_name, const char* pfs_file_name);
	void ImportMeshData(const char* file_name);
	void ImportVertexShader(const char* file_name);
	void ImportFragmentShader(const char* file_name);
	void Render();
	GLuint getShaderProgramID();
	size_t getMeshPointCount();
};
#endif