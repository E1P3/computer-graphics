#version 330

in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 tex;
in vec3 tangent;
in vec3 bitangent;

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

	mat3 normalMatrix = transpose(mat3(model));
	vec3 T = normalize(normalMatrix * tangent);
	vec3 N = normalize(normalMatrix * vertex_normal);
	T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N)); 

    LightPos = TBN * lightPos;
    ViewPos  = TBN * viewPos;
    FragPos  = TBN * FragPos;

	gl_Position = proj * view * model * vec4(vertex_position,1.0);

}


  