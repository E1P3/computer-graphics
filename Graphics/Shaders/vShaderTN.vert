#version 330

in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 tex;
in vec3 tangent;
in vec3 bitangent;


out vec2 Texture;
out mat3 TBN_mat;
out vec3 FragPos;
out vec3 LightPos;
out vec3 ViewPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

void main(){

	vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(vertex_normal,    0.0)));
    mat3 TBN = mat3(T, B, N);
    
    Texture = tex;
    TBN_mat = TBN;
    LightPos = lightPos;
    ViewPos  = viewPos;
    FragPos = vec3(model * vec4(vertex_position, 1.0));

	gl_Position = proj * view * vec4(FragPos,1.0);

}


  