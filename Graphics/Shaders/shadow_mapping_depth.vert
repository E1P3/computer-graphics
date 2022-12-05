#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 5) in ivec4 vertex_boneIds; 
layout (location = 6) in vec4 vertex_weights;
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

uniform int  isAnimated;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    vec4 finalPosition = vec4(0.0f);

    if(isAnimated == 1){
        finalPosition = vec4(0.0f);
        for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            if(vertex_boneIds[i] == -1) 
                continue;
            if(vertex_boneIds[i] >=MAX_BONES) 
            {
                finalPosition = vec4(vertex_position,1.0f);
                break;
            }
            vec4 localPosition = finalBonesMatrices[vertex_boneIds[i]] * vec4(vertex_position,1.0f);
            finalPosition += localPosition * vertex_weights[i];
        }
    } else {
        finalPosition = vec4(vertex_position, 1.0);
    }

    gl_Position = lightSpaceMatrix * model * finalPosition;
}  