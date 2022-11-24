#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out vec3 FragPos;
out vec3 LightPos;
out vec3 ViewPos;
out vec2 Texture;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

void main(){

	Texture = tex;
	FragPos = vec3(model * vec4(vertex_position, 1.0));

	vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
	vec3 N = normalize(vec3(model * vec4(vertex_normal,    0.0)));
	T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N)); 

    LightPos = TBN * lightPos;
    ViewPos  = TBN * viewPos;
    FragPos  = TBN * FragPos;

	gl_Position = proj * view * model * vec4(vertex_position,1.0);

}


  