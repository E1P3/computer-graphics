#ifndef POINTLIGHT_CLASS_H
#define POINTLIGHT_CLASS_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class PointLight {
public:
	int index;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(0.5f);
	glm::vec3 ambient = glm::vec3(0.01f);
	glm::vec3 specular = glm::vec3(0.22f);
	float constant = 1.0f;
	float linear = 0.22f;
	float quadratic = 0.20f;
	float strength = 1.0f;

	PointLight() {}
	PointLight(glm::vec3 pos) {
		position = pos;
	}

	void Update(Shader* shader) {
		shader->SetVector3f("pointLights[" + std::to_string(index) + "].position", position, true);
		shader->SetVector3f("pointLights[" + std::to_string(index) + "].ambient", ambient, true);
		shader->SetVector3f("pointLights[" + std::to_string(index) + "].diffuse", diffuse, true);
		shader->SetVector3f("pointLights[" + std::to_string(index) + "].specular", specular, true);
		shader->SetFloat("pointLights[" + std::to_string(index) + "].constant", constant, true);
		shader->SetFloat("pointLights[" + std::to_string(index) + "].linear", linear, true);
		shader->SetFloat("pointLights[" + std::to_string(index) + "].quadratic", quadratic, true);
		shader->SetFloat("pointLights[" + std::to_string(index) + "].strength", strength, true);
	}

};

#endif