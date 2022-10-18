#ifndef GAMEOBJECT_CLASS_H
#define GAMEOBJECT_CLASS_H

#include "MeshRenderer.h"
#include "Camera.h"
#include <glm/glm.hpp>

class GameObject {
	
public:
	Camera* camera;
	glm::vec3 position;
	MeshRenderer Renderer;
	int width;
	int height;

	GameObject() {}
	GameObject(float positionx, float positiony, float positionz, int width, int height);
	void Draw();
	void Move(float x, float y, float z);
	void SetCamera(Camera* camera);
	void RenderSetup(const char* mesh_file_name, const char* pvs_file_name, const char* pfs_file_name);
};
#endif
