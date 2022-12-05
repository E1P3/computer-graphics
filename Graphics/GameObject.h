#ifndef GAMEOBJECT_CLASS_H
#define GAMEOBJECT_CLASS_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "Camera.h"
#include "shader.h"
#include "Model.h"
#include "Animator.h"

class GameObject {
	
public:
	Model model;
	Animator* animator = NULL;
	bool  isAnimatied = false;
	bool  isLight = false;
	float PosX = 0.0f;
	float PosY = 0.0f;
	float PosZ = 0.0f;
	float RotX = 0.0f;
	float RotY = 0.0f;
	float RotZ = 0.0f;
	float ScaleX = 1.0f;
	float ScaleY = 1.0f;
	float ScaleZ = 1.0f;
	glm::mat4 transformMatrix = glm::mat4(1.0f);;
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	glm::mat4 transform = glm::mat4(1.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	GameObject() {}

	GameObject(string const& model_filename) {
		this->model = Model(model_filename);
	}

	void Draw(Shader* shader) {
		shader->SetMatrix4("model", transform, false);
		if (isAnimatied) {
			auto transforms = animator->GetFinalBoneMatrices();
			for (int i = 0; i < transforms.size(); ++i)
				shader->SetMatrix4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i], false);
			shader->SetInteger("isAnimated", 1, false);
		}
		else {
			shader->SetInteger("isAnimated", 0, false);
		}
		model.Draw(shader->ID);
	}

	void Move(float posx = 0.0f, float posy = 0.0f, float posz = 0.0f) {
		transformMatrix = glm::translate(transformMatrix, glm::vec3(posx, posy, posz));
		position.x += posx;
		position.y += posy;
		position.z += posz;
		RecalculateTransform();
	}

	void Scale(float scalex = 1.0f, float scaley = 1.0f, float scalez = 1.0f) {
		scaleMatrix = glm::scale(scalex, scaley, scalez);
		RecalculateTransform();
	}

	void Rotate(float rotx = 0.0f, float roty = 0.0f, float rotz = 0.0f) {
		RotX += rotx;
		RotY += roty;
		RotZ += rotz;
		if (RotX <= 360.0f)
			RotX -= 360.0f;
		if (RotY <= 360.0f)
			RotY -= 360.0f;
		if (RotZ <= 360.0f)
			RotZ -= 360.0f;

		RecalculateTransform();
	}

	void SetRotation(float rotx = 0.0f, float roty = 0.0f, float rotz = 0.0f) {
		RotX = rotx;
		RotY = roty;
		RotZ = rotz;
		if (RotX <= 360.0f)
			RotX -= 360.0f;
		if (RotY <= 360.0f)
			RotY -= 360.0f;
		if (RotZ <= 360.0f)
			RotZ -= 360.0f;

		RecalculateTransform();
	}

	void RecalculateTransform() {
		transform = transformMatrix;
		transform = glm::rotate(transform, RotX, glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, RotY, glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, RotZ, glm::vec3(0.0f, 0.0f, 1.0f));
		transform *= scaleMatrix;
	}

	void setAnimator(Animator* animator) {
		this->animator = animator;
	}

};
#endif
