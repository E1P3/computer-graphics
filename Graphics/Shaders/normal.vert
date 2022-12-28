#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_uv;
layout (location = 3) in vec3 vertex_tangent;
layout (location = 4) in vec3 vertex_bitangent;
layout (location = 5) in ivec4 vertex_boneIds; 
layout (location = 6) in vec4 vertex_weights;

out VERTEX{
    vec3 fragment_position;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec2 uv;
    vec4 fragment_position_light_space;
    mat3 TBN;
} vertex;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform int  isAnimated;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    vec4 finalPosition = vec4(0.0f);
    vec3 finalNormal;

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
            finalNormal = mat3(finalBonesMatrices[vertex_boneIds[i]]) * vertex_normal;
        }
    } else {
        finalPosition = vec4(vertex_position, 1.0);
        finalNormal = vertex_normal;
    }

    vec3 T = normalize(vec3(model * vec4(vertex_tangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(vertex_bitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(vertex_normal,    0.0)));
    vertex.TBN = mat3(T, B, N);

    vertex.fragment_position = vec3(model * finalPosition);
    vertex.normal = transpose(inverse(mat3(model))) * finalNormal;
    vertex.uv = vertex_uv;
    vertex.fragment_position_light_space = lightSpaceMatrix * vec4(vertex.fragment_position, 1.0);
    vertex.tangent = vertex_tangent;
    vertex.bitangent = vertex_bitangent;

    gl_Position = proj * view * model * finalPosition;
}
