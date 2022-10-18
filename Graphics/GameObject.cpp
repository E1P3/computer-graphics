#include "GameObject.h"

GameObject::GameObject(float positionx, float positiony, float positionz, int width, int height) {
	this->position = glm::vec3(positionx, positiony, positionz);
	this->width = width;
	this->height = height;
}

void GameObject::Draw() {
	this->Renderer.Draw(width, height, this->position, 0.0f, camera);
}

void GameObject::Move(float x, float y, float z) {
	this->position = glm::vec3(position.x + x, position.y + y, position.z + z);
}

void GameObject::SetCamera(Camera* camera) {
	this->camera = camera;
}

void GameObject::RenderSetup(const char* mesh_file_name, const char* pvs_file_name, const char* pfs_file_name) {
	this->Renderer = MeshRenderer(mesh_file_name, pvs_file_name, pfs_file_name);
}
