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

# define M_PI           3.14159265358979323846  /* pi */

enum angleType {
	RAD,
	DEG
};

class GameObject {
	
public:
	Model* model;
	Animator animator;
	bool  isAnimatied = false;
	bool  hasNormalMap = false;
	bool  hasShadow = true;
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
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	glm::mat4 transform = glm::mat4(1.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	GameObject() {}

	GameObject(Model* model) {
		this->model = model;
	}

	void SwitchModel(Model* model) {
		this->model = model;
	}

	void Draw(Shader* shader) {
		shader->SetMatrix4("model", transform, false);
		if (isAnimatied) {
			auto transforms = animator.GetFinalBoneMatrices();
			for (int i = 0; i < transforms.size(); ++i)
				shader->SetMatrix4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i], false);
			shader->SetInteger("isAnimated", 1, false);
		}
		else {
			shader->SetInteger("isAnimated", 0, false);
		}

		if (hasNormalMap) {
			shader->SetInteger("hasNormalMap", 1, false);
		}
		else {
			shader->SetInteger("hasNormalMap", 0, false);
		}

		if (hasShadow) {
			shader->SetInteger("hasShadow", 1, false);
		}
		else {
			shader->SetInteger("hasShadow", 0, false);
		}

		model->Draw(shader->ID);
	}

	void Move(float posx = 0.0f, float posy = 0.0f, float posz = 0.0f) {
		transformMatrix = glm::translate(transformMatrix, glm::vec3(posx, posy, posz));
		position.x += posx;
		position.y += posy;
		position.z += posz;
		RecalculateTransform();
	}

	void Move(glm::vec3 _velocity) {
		transformMatrix = glm::translate(transformMatrix, _velocity);
		position = position + _velocity;
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

	void RecalculateTransform() {
		transform = transformMatrix;
		transform = glm::rotate(transform, RotX, glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, RotY, glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, RotZ, glm::vec3(0.0f, 0.0f, 1.0f));
		transform *= scaleMatrix;
	}

	void setAnimation(Animation* animation) {
		animator = Animator(animation);
	}

	float distanceTo(glm::vec3 _position) {
		return sqrt(pow(position.x - _position.x,2) + pow(position.y - _position.y, 2) + pow(position.z - _position.z, 2));
	}

	void Step(float delta) {
		if (velocity == glm::vec3(0.0f))
			return;
		RotY = atan2(normalize(velocity).x, normalize(velocity).z) * 180 / M_PI;
		Move(velocity * delta);
		velocity = glm::vec3(0.0f);
	}

	void updateVelocity(glm::vec3 _vector) {
		velocity = velocity + _vector;
	}

	void aimToPoint(glm::vec3 position, float _velocity) {
		updateVelocity(getDirectionToPoint(position) * _velocity);
	}

	void setDirection(float angle, angleType type) {
		float x, z;
		if (type == RAD) {
			z = cos(angle);
			x = sin(angle);
		}
		else if (type == DEG) {
			float rad = angle / 180 * M_PI;
			z = cos(rad);
			x = sin(rad);
		}
		direction = glm::vec3(x, 0.0f, z);
		RotY = atan2(direction.x, direction.z) * 180 / M_PI;
		cout << angle << " " << RotY << " " << direction.x << "\n";
		RecalculateTransform();
	}

	void updateDirection(float angle ) {
		setDirection(RotY + angle, DEG);
	}

	glm::vec3 getDirectionToPoint(glm::vec3 point) {

		if (point == position) {
			return glm::vec3(0, 0, 0);
		}
		else {
			return -normalize(glm::vec3(position.x - point.x, position.y - point.y, position.z - point.z));
		}
	}

	void setRandDirection() {
		float angle = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 - 1) * M_PI;
		float z = cos(angle);
		float x = sin(angle);
		Rotate(0.0f, angle * 180 / M_PI, 0.0f);
		direction = glm::vec3(x, 0.0f, z);
	 }

	glm::vec3 normalize(const glm::vec3& v)
	{
		if (v == glm::vec3(0.0f))
			return v;
		float length_of_v = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
		return glm::vec3(v.x / length_of_v, v.y / length_of_v, v.z / length_of_v);
	}
};
#endif
