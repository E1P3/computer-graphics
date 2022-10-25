#ifndef GAMEOBJECT_CLASS_H
#define GAMEOBJECT_CLASS_H

#include "MeshRenderer.h"
#include "Camera.h"
#include <glm/glm.hpp>

class GameObject {
	
public:
	MeshRenderer Renderer;

	GameObject() {}
	void RenderSetup(const char* mesh_file_name, const char* pvs_file_name, const char* pfs_file_name);
};
#endif
