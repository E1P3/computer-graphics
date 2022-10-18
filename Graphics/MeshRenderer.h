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
#include <glm/glm.hpp>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

#include "shader.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"

using namespace glm;

class MeshRenderer
{

typedef struct ModelData
{
	size_t mPointCount = 0;
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;
	std::vector<glm::vec2> mTextureCoords;
} ModelData;

private:
	ModelData mesh_data;
	const char* PVS;
	const char* PFS;
	char* readShaderSource(const char* shaderFile);

public:
	MeshRenderer() {}
	MeshRenderer(const char* mesh_file_name, const char* pvs_file_name, const char* pfs_file_name);
	void ImportMeshData(const char* file_name);
	void ImportVertexShader(const char* file_name);
	void ImportFragmentShader(const char* file_name);
	void Render();
	void Draw(int width, int height, glm::vec3 position, float angle, Camera* camera);
	GLuint getShaderProgramID();
	size_t getMeshPointCount();
	VAO mesh_vao;
	Shader mesh_shader;
};
#endif