#version 330

in vec3 vertex_position;
in vec3 vertex_normal;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

uniform vec3 lightPos;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

void main(){  
  gl_Position = proj * view * model * vec4(vertex_position,1.0);
  FragPos = vec3(model * vec4(vertex_position, 1.0));
  Normal = mat3(transpose(inverse(model))) * vertex_normal;
  LightPos = lightPos;
}


  