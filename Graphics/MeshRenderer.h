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
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

#include "shader.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "Material.h"

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
	float PosX = 0.0f;
	float PosY = 0.0f;
	float PosZ = 0.0f;
	float RotX = 0.0f;
	float RotY = 0.0f;
	float RotZ = 0.0f;
	float ScaleX = 1.0f;
	float ScaleY = 1.0f;
	float ScaleZ = 1.0f;
	VAO mesh_vao;
	Shader mesh_shader;
	glm::mat4 transformMatrix = glm::mat4(1.0f);;
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	Material mesh_mat = Material();
	void RecalculateTransform();

public:
	MeshRenderer() {}
	MeshRenderer(const char* mesh_file_name, const char* pvs_file_name, const char* pfs_file_name);
	void ImportMeshData(const char* file_name);
	void ImportVertexShader(const char* file_name);
	void ImportFragmentShader(const char* file_name);
	void Render();
	void SetMaterial(Material mat);
	void Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos, glm::vec3 lightPos);
	void Move(float posx, float posy, float posz);
	void Rotate(float rotx, float roty, float rotz);
	void SetRotation(float rotx, float roty, float rotz);
	void Scale(float scalex, float scaley, float scalez);
	GLuint getShaderProgramID();
	size_t getMeshPointCount();
	glm::mat4 transform = glm::mat4(1.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

};
#endif