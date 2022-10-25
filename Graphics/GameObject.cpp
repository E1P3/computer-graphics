#include "GameObject.h"

using namespace glm;

void GameObject::RenderSetup(const char* mesh_file_name, const char* pvs_file_name, const char* pfs_file_name) {
	this->Renderer = MeshRenderer(mesh_file_name, pvs_file_name, pfs_file_name);
}
