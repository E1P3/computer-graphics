#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//default material values

const glm::vec3 def_color = glm::vec3(1.0f, 1.0f, 1.0f);
const float def_ambientStrength = 0.1f;
const float def_diffStrength = 0.7f;
const float def_specularStrength = 0.2f;

class Material
{
public:
	glm::vec3 color;
	float ambientStrength;
	float diffStrength;
	float specularStrength;

	Material(glm::vec3 _color = def_color, float _ambientStrength = def_ambientStrength, float _diffStrength = def_diffStrength, float _specularStrength = def_specularStrength)
	{
		color = _color;
		ambientStrength = _ambientStrength;
		diffStrength = _diffStrength;
		specularStrength = _specularStrength;
	}
};

#endif